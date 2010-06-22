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

class Rot2( var sin: Double, var cos: Double){
  def this( alpha:Double) = this( Math.sin( alpha ), math.cos( alpha ) )
  def normalize() = {
    val n = 1.0 / Math.sqrt( MyMath.sqr(sin) + MyMath.sqr(cos) );
    sin /= n;
    cos /= n;
  }
  def angle = Math.atan2( cos, sin )
  override def toString() = "Rot2("+angle+")"
  override def clone() = new Rot2( sin, cos )

  def add( r: Rot2 ) = {
    val cos1 = cos*r.cos - sin*r.sin;
    val sin1 = sin*r.cos + cos*r.sin;
    sin = sin1;
    cos = cos1;
  }
  def set( r: Rot2 ) = {
    cos = r.cos;
    sin = r.sin;
  }
  def +( r: Rot2 ): Rot2 = {
    val t=this.clone()
    t add r
    return t
  }
}

object Main{
  def main(args: Array[String]) = {
    println( new Vec2(10,20) )
    val v = new Vec2(20,10)
    val v1 = new Vec2( 20, 30)
    println( v.len2(), v.len(), v.clone() )
    println( "Dist:"+v+"->"+v+" = "+(v1 dist v) )

    val r = new Rot2( 1 )
    println( "Rotation:"+( r + new Rot2(-2) ))
  }
}
