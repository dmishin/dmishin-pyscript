import java.lang.Math
object MyMath{
  def sqr( x: Double ) = x*x;
}

class Vec2( var x:Double, var y:Double) {
  override def toString() = "Vec2("+x+","+y+")"
  def add ( v: Vec2, k: Double)={
    x += v.x*k;
    y += v.y*k;
  }
  def len2() = MyMath.sqr( x)+ MyMath.sqr(y)
  def len() = Math.sqrt( len2 )
  override def clone() = new Vec2( x, y )
  def dist2( that:Vec2 ) = MyMath.sqr(x-that.x)+ MyMath.sqr(y-that.y)
  def dist( that:Vec2 ) = Math.sqrt( dist2( that ) )
}

object Main{
  def main(args: Array[String]) = {
    println( new Vec2(10,20) )
    val v = new Vec2(20,10)
    val v1 = new Vec2( 20, 30)
    println( v.len2(), v.len(), v.clone() )
    println( "Dist:"+v+"->"+v+" = "+(v1 dist v) )
  }
}
