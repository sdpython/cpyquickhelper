"""
@file
@brielf Shortcut to *parallel*.
"""
from threading import Thread
from .thread import kill_thread


class KThread(Thread):
    """
    Add methods *kill* to :epkg:`*py:threading:Thread`.
    """

    def kill(self):
        """
        Kills the thread if not alreayd finished.
        Source: `Python-Kill-Thread-Extension <https://github.com/munawarb/Python-Kill-Thread-Extension>`_.

        @return     None if not alive, exit code otherwise.
        """
        if self.is_alive():
            return kill_thread(self.ident)
        return None
