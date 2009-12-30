import unittest
import numtheor

class TestNumtheor(unittest.TestCase):
    def test_pfac(self):
        self.assertEqual( numtheor.pfac(2) ,[2])
        self.assertEqual( numtheor.pfac(6) , [2,3])
        self.assertEqual( numtheor.pfac(8*37*11) , [2,2,2,11,37])

if __name__=="__main__":
    unittest.main()


