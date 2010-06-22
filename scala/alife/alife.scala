import java.lang.Math
object MyMath{
  def sqr( x: Double ) = x*x;
}

class Vec2( var x:Double, var y:Double) {
  override def toString() = "Vec2("+x+","+y+")"
}

object Main{
  def main(args: Array[String]) = {
    println( new Vec2(10,20) )
  }
}
