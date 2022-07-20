"""
@file
@brief Addition for :epkg:`pandas`.
"""
from itertools import chain
from typing import Sequence, Type
import numpy
from pandas import Series
from pandas.api.extensions import (
    register_series_accessor, ExtensionDtype, register_extension_dtype)
from pandas.core.arrays.base import ExtensionArrayT
from pandas.arrays import PandasArray
from pandas.core.arrays.numpy_ import PandasDtype
from .weighted_number import WeightedDouble  # pylint: disable=E0611


class WeightedSeriesDtype(ExtensionDtype):
    """
    Defines a custom type for a @see cl WeightedSeries.
    """

    dtype = numpy.dtype(WeightedDouble)

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

    def __repr__(self):
        "usual"
        return "WeightedSeriesDtype()"

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
        if not string.startswith("WD"):  # pragma no cover
            raise TypeError(f"Unable to parse '{string}'")
        val = string[2:].strip('() ').split(",")
        if len(val) == 1 and val[0]:
            val = float(val[0])
        elif len(val) == 2:
            val = float(val[0]), float(val[1])
        elif len(val) == 0 or (len(val) == 1 and val[0] == ''):
            val = numpy.nan
        else:  # pragma no cover
            raise TypeError(f"Unable to parse '{string}'")
        if isinstance(val, tuple):
            if len(val) != 2:  # pragma no cover
                raise TypeError(f"Unable to parse '{string}'")
            return WeightedDouble(val[0], val[1])
        return WeightedDouble(val)

    @classmethod
    def construct_array_type(cls):
        """
        Return the array type associated with this dtype.

        Returns
        -------
        type
        """
        return WeightedArray


register_extension_dtype(WeightedSeriesDtype)


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
        return self._new_series(lambda s: numpy.isnan(s.value))

    def _new_series(self, fct):
        if len(self) == 0:  # pragma no cover
            raise ValueError("Series cannot be empty.")
        if isinstance(self.obj, WeightedArray) or isinstance(self.obj[0], WeightedDouble):
            return WeightedArray([fct(s) for s in self.obj],
                                 index=self.obj.index, dtype=float)
        raise TypeError(  # pragma no cover
            "Unexpected type, array is '{0}', first element is '{1}'".format(
                type(self.obj), type(self.obj[0])))


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
            return getattr(self, attr)
        if hasattr(WeightedDoubleAccessor, attr):
            obj = WeightedDoubleAccessor(self)
            return getattr(obj, attr)
        if attr == '_ndarray':
            return numpy.array(self)
        raise AttributeError(f"Unkown attribute '{attr}'")


class WeightedArray(PandasArray):
    """
    Implements an array holding @see WeightedDouble numbers.
    This leverages a new concept introduced in :epkg:`pandas` 0.24
    implemented in class :epkg:`PandasArray`. It can be used
    to define a new column type in a dataframe.
    """

    def __init__(self, *args, **kwargs):
        """
        Overwrites the constructor to force
        *dtype* to be @see cl WeightedSeriesDtype.
        """
        if "data" in kwargs and isinstance(kwargs["data"], WeightedSeries):
            serie = kwargs["data"]
        elif len(args) == 1 and isinstance(args[0], numpy.ndarray):
            PandasArray.__init__(self, args[0])
        else:
            serie = WeightedSeries(*args, **kwargs)
            PandasArray.__init__(self, serie._ndarray)

    @property
    def dtype(self):
        """
        Returns @see cl WeightedSeriesDtype.
        """
        return self._dtype

    @property
    def name(self):
        """
        A string identifying the data type.
        Will be used for display in, e.g. ``Series.dtype``
        """
        return "WeightedArray"

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

    def isna(self):
        "is nan?"
        return numpy.array([numpy.isnan(s.value) for s in self])

    @classmethod
    def _concat_same_type(cls: Type[ExtensionArrayT],  # pylint: disable=W0221
                          to_concat: Sequence[ExtensionArrayT]) -> ExtensionArrayT:
        """Concatenate multiple array

        Parameters
        ----------
        to_concat : sequence of this type

        Returns
        -------
        @see cl WeightedArray
        """
        for s in to_concat:
            if not isinstance(s.dtype, (WeightedSeriesDtype, object)):
                raise TypeError(  # pragma no cover
                    "All arrays must be of type WeightedSeriesDtype not {}-{}".format(
                        type(s), type(s.dtype)))
        return WeightedArray(list(chain(*to_concat)))

    @classmethod
    def _from_sequence(cls, scalars, *, dtype=None, copy=False):
        if isinstance(dtype, PandasDtype):
            dtype = dtype._dtype

        result = numpy.asarray(scalars, dtype=dtype)
        if copy and result is scalars:
            result = result.copy()
        return cls(result)
