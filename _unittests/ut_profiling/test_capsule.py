"""
@brief      test log(time=3s)
"""
import unittest
from pyquickhelper.pycode import ExtTestCase
from cpyquickhelper.profiling._event_profiler_c import (  # pylint: disable=E0611
    dummy_capsule, get_memory_content)


class TestCapsule(ExtTestCase):

    def test_capsule(self):
        caps = dummy_capsule("fhg")
        self.assertIn("fhg", str(caps))
        content = get_memory_content(caps, "fhg", 128)
        ints = list(content)
        self.assertEqual(ints[::4], list(range(32)))
        self.assertEqual(ints[1::4], [0] * 32)


if __name__ == "__main__":
    unittest.main()
