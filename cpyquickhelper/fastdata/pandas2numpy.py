"""
@file
@brief Fast data manipulations.
"""
import pandas


def df2array(df, check=True):
    """
    Converts a dataframe into a :epkg:`numpy:array`
    without copying. :epkg:`pandas` is merging
    consecutive columns sharing the same type
    into one memory block. The function can be used
    only if the data is stored in one block and one type
    as a consequence.

    @param      df      dataframe
    @param      check   verifies the operation can be done (True)
                        or skip verification (False)
    @return             :epkg:`numpy:array`

    See `data member <https://pandas.pydata.org/pandas-docs/stable/search.html?q=pointer&check_keywords=yes&area=default>`_,
    `_data <https://github.com/pandas-dev/pandas/blob/master/pandas/core/frame.py#L322>`_.

    .. seealso:: @see fn df2array
    """
    if check:
        if not isinstance(df, pandas.DataFrame):
            raise TypeError("df is not a pandas.DataFrame")  # pragma: no cover
        if len(df._data.blocks) != 1:
            raise ValueError(
                "The dataframe has many block of data. There should be only one column type.")
    return df._data.blocks[0].values


def df2arrays(df, sep=",", check=True):
    """
    Converts a dataframe into a list of
    a list of tuple *(column name, :epkg:`numpy:array`)*
    without copying. :epkg:`pandas` is merging
    consecutive columns sharing the same type
    into one memory block. That's what the function extracts

    @param      df      dataframe
    @param      check   verifies the operation can be done (True)
                        or skip verification (False)
    @param      sep     columns separator
    @return             a list of tuple ``(column, array)``

    Example:

    .. runpython::
        :showcode:

        from pandas import DataFrame
        from cpyquickhelper.fastdata import df2arrays

        df = DataFrame([dict(a=3.4, b=5.6, c="e"),
                        dict(a=3.5, b=5.7, c="r")])
        arr = df2arrays(df)
        print(arr)


    .. seealso:: @see fn df2array
    """
    if check:
        if not isinstance(df, pandas.DataFrame):
            raise TypeError("df is not a pandas.DataFrame")  # pragma: no cover

    cols = df.columns
    res = []
    pos = 0
    for b in df._data.blocks:
        name = sep.join(cols[pos:pos + b.shape[1]])
        res.append((name, b.values))
        pos += b.shape[1]
    return res
