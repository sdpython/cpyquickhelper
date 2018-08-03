"""
@file
@brief Shortcut to *numbers*.
"""
from itertools import chain
from numpy import isnan, dtype, nan
from pandas import Series
from pandas.core.dtypes.base import ExtensionDtype
from pandas.api.extensions import register_series_accessor
from pandas.api.extensions import ExtensionArray
from .weighted_number import WeightedDouble


class WeightedSeriesDtype(ExtensionDtype):
    """
    Defines a custom type for a @see cl WeightedSeries.
    """

    dtype = dtype(WeightedDouble)

    def __str__(self):
        """
        usual
        """
        return self.name

    @property
    def type(self):
        # type: () -> type
        """The scalar type for the array, e.g. ``int``
        It's expected ``ExtensionArray[item]`` returns an instance
        of ``ExtensionDtype.type`` for scalar ``item``.
        """
        return WeightedSeriesDtype

    @property
    def kind(self):
        # type () -> str
        """
        A character code (one of 'biufcmMOSUV'), default 'O'
        This should match the NumPy dtype used when the array is
        converted to an ndarray, 'O' in this case.
        type.

        See Also
        --------
        numpy.dtype.kind
        """
        return WeightedSeriesDtype.dtype.kind

    @property
    def name(self):
        """
        A string identifying the data type.
        Will be used for display in, e.g. ``Series.dtype``
        """
        return "WeightedDouble"

    @classmethod
    def construct_from_string(cls, string):
        """
        Attempt to construct this type from a string.
        Parameters
        ----------
        string : str

        Returns
        -------
        self : instance of 'WeightedDouble'

        Raises
        ------
        TypeError
            If a class cannot be constructed from this 'string'.
        """
        if not string.startswith("WD"):
            raise TypeError("Unable to parse '{0}'".format(string))
        val = string[:2].strip('(', ')').split(",")
        if len(val) == 1:
            val = float(val)
        elif len(val) == 2:
            val = float(val[0]), float(val[1])
        elif len(val) == 0:
            val = nan
        else:
            raise TypeError("Unable to parse '{0}'".format(string))
        if isinstance(val, tuple):
            if len(val) != 2:
                raise TypeError("Unable to parse '{0}'".format(string))
            return WeightedDouble(val[0], val[1])
        else:
            return WeightedDouble(val)


@register_series_accessor("wdouble")
class WeightedDoubleAccessor:
    """
    Extends :epkg:`pandas` with new accessor for
    series based on @see cl WeightedDouble.
    """

    def __init__(self, obj):
        self.obj = obj

    def __len__(self):
        return len(self.obj)

    @property
    def value(self):
        "Returns the values."
        return self._new_series(lambda s: s.value)

    @property
    def weight(self):
        "Returns the weights."
        return self._new_series(lambda s: s.weight)

    def isnan(self):
        "Tells if values are missing."
        return self._new_series(lambda s: isnan(s.value))

    def _new_series(self, fct):
        if len(self) == 0:
            raise ValueError("Series cannot be empty.")
        v1 = self.obj[0]
        if isinstance(v1, WeightedDouble):
            return Series([fct(s) for s in self.obj], index=self.obj.index, dtype=float)
        else:
            raise TypeError("Unexpected type '{0}'".format(type(v1)))


class WeightedSeries(Series):
    """
    Implements a series holding @see WeightedDouble numbers.
    Does not add anything to *Series*.
    """

    def __init__(self, *args, **kwargs):
        """
        Overwrites the constructor to force
        dtype to be @see cl WeightedSeriesDtype.
        """
        dt = kwargs.pop('dtype', WeightedSeriesDtype())
        Series.__init__(self, *args, dtype=dt, **kwargs)

    def __getattr__(self, attr):
        """
        Tries first to see if class *Series* has this attribute
        and then tries @see cl WeightedDoubleAccessor.
        """
        if hasattr(Series, attr):
            return self.__dict__[attr]
        elif hasattr(WeightedDoubleAccessor, attr):
            obj = WeightedDoubleAccessor(self)
            return getattr(obj, attr)
        else:
            raise AttributeError("Unkown attribute '{0}'".format(attr))


class WeightedArray(ExtensionArray):
    """
    Implements an array holding @see WeightedDouble numbers.
    This leverages a new concept introduced in :epkg:`pandas` 0.23
    implemented in class :epkg:`ExtensionArray`. It can be used
    to define a new column type in a dataframe.
    """

    def __init__(self, *args, **kwargs):
        """
        Overwrites the constructor to force
        dtype to be @see cl WeightedSeriesDtype.
        """
        if "data" in kwargs and isinstance(kwargs["data"], WeightedSeries):
            self._data = kwargs["data"]
        else:
            self._data = WeightedSeries(*args, **kwargs)
        self._dtype = kwargs.get("dtype", WeightedSeriesDtype())

    @property
    def dtype(self):
        """
        Returns @see cl WeightedSeriesDtype.
        """
        return self._dtype

    def __len__(self):
        "Returns the length of the series."
        return len(self._data)

    def __getattr__(self, attr):
        """
        Forward
        """
        if hasattr(WeightedArray, attr):
            return self.__dict__[attr]
        else:
            return getattr(self._data, attr)

    def copy(self, deep=False):
        # type: (bool) -> ExtensionArray
        """Return a copy of the array.

        Parameters
        ----------
        deep : bool, default False
            Also copy the underlying data backing this array.

        Returns
        -------
        @see cl WeightedArray
        """
        data = self._data.copy(deep=deep)
        return WeightedArray(data=data, dtype=self.dtype)

    def __getitem__(self, item):
        # type (Any) -> Any
        """Select a subset of self.

        Parameters
        ----------
        item : int, slice, or ndarray
            * int: The position in 'self' to get.

            * slice: A slice object, where 'start', 'stop', and 'step' are
              integers or None

            * ndarray: A 1-d boolean NumPy ndarray the same length as 'self'

        Returns
        -------
        item : scalar or @see cl WeightedArray

        Notes
        -----
        For scalar ``item``, return a scalar value suitable for the array's
        type. This should be an instance of ``self.dtype.type``.

        For slice ``key``, return an instance of ``ExtensionArray``, even
        if the slice is length 0 or 1.

        For a boolean mask, return an instance of ``ExtensionArray``, filtered
        to the values where ``item`` is True.
        """
        return self._data[item]

    def __setitem__(self, key, value):
        # type: (Union[int, np.ndarray], Any) -> None
        """Set one or more values inplace.

        This method is not required to satisfy the pandas extension array
        interface.

        Parameters
        ----------
        key : int, ndarray, or slice
            When called from, e.g. ``Series.__setitem__``, ``key`` will be
            one of

            * scalar int
            * ndarray of integers.
            * boolean ndarray
            * slice object

        value : ``WeightedSeriesDtype.type``,
            ``Sequence[WeightedSeriesDtype.type]``,
            or object value or values to be set of ``key``.
        """
        self._data[key] = value

    def __add__(self, other):
        "Addition"
        return WeightedArray([a + b for a, b in zip(self, other)])

    def __sub__(self, other):
        "Soustraction"
        return WeightedArray([a - b for a, b in zip(self, other)])

    def __mul__(self, other):
        "Multiplication"
        return WeightedArray([a * b for a, b in zip(self, other)])

    def __truediv__(self, other):
        "Division"
        return WeightedArray([a / b for a, b in zip(self, other)])

    @classmethod
    def _concat_same_type(cls, to_concat):
        # type: (Sequence[ExtensionArray]) -> ExtensionArray
        """Concatenate multiple array

        Parameters
        ----------
        to_concat : sequence of this type

        Returns
        -------
        @see cl WeightedArray
        """
        for s in to_concat:
            if not isinstance(s.dtype, WeightedSeriesDtype):
                raise TypeError(
                    "All arrays must be of type WeightedSeriesDtype")
        return WeightedArray(list(chain(*to_concat)))
