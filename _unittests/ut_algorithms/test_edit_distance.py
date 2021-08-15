# -*- coding: utf-8 -*-
"""
@brief      test tree node (time=4s)
"""
import unittest
from pyquickhelper.pycode import ExtTestCase
from cpyquickhelper.algorithms.edit_distance import edit_distance_string


class TestTextDiff(ExtTestCase):

    def test_edit_distance_string(self):
        s1 = "ABCD"
        s2 = "ACD"
        d, aligned = edit_distance_string(s1, s2)
        self.assertEqual(d, 1)
        self.assertEqual(aligned, [(0, 0), (2, 1), (3, 2)])
        d, aligned = edit_distance_string(s2, s1)
        self.assertEqual(d, 1)
        self.assertEqual(aligned, [(0, 0), (1, 2), (2, 3)])

    def test_edit_distance_string_empty(self):
        s1 = ""
        s2 = "ACD"
        d, aligned = edit_distance_string(s1, s2)
        self.assertEqual(d, 3)
        self.assertEqual(aligned, [])
        s1 = "ABCD"
        s2 = ""
        d, aligned = edit_distance_string(s1, s2)
        self.assertEqual(d, 4)
        self.assertEqual(aligned, [])


if __name__ == "__main__":
    # TestTextDiff().test_edit_distance_string()
    unittest.main()
