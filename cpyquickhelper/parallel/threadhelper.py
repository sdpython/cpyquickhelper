"""
@file
@brief Shortcut to *parallel*.
"""
from threading import Thread
from .threader import kill_thread  # pylint: disable=E0611


class KThread(Thread):
    """
    Add methods *kill* to :epkg:`*py:threading:Thread`.
    """

    def kill(self):
        """
        Kills the thread if not alreayd finished.
        Source: `Python-Kill-Thread-Extension <https://github.com/munawarb/Python-Kill-Thread-Extension>`_.
        The function still does not work. The found example
        is not really working and it is not safe anyway as it will
        leave the garbage collector in an unstable state.

        @return     None if not alive, exit code otherwise.
        """
        if self.is_alive():  # pragma: no cover
            return kill_thread(self.ident)
        return None  # pragma: no cover
