"""
@file
@brief Shortcut to *numbers*.
"""
from numpy import isnan, dtype, nan
from pandas import Series
from pandas.core.dtypes.base import ExtensionDtype
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
        return WeightedSeriesDtype.dtype.type

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
        return "WD"

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


class WeightedSeries(Series):
    """
    Implements a series holding @see WeightedDouble numbers.
    """

    def __init__(self, *args, **kwargs):
        """
        Overwrites the constructor to force
        dtype to be @see cl WeightedSeriesDtype.
        """
        dt = kwargs.pop('dtype', WeightedSeriesDtype())
        Series.__init__(self, *args, dtype=dt, **kwargs)

    @property
    def value(self):
        """
        Returns the value for all elements in the series.
        """
        return Series([s.value for s in self], index=self.index, dtype=float)

    @property
    def weight(self):
        """
        Returns the weight for all elements in the series.
        """
        return Series([s.weight for s in self], index=self.index, dtype=float)

    def isnan(self):
        """
        Returns the value for all elements in the series.
        """
        return Series([isnan(s.value) for s in self], index=self.index, dtype=bool)
