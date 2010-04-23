import scala.actors._
import scala.actors.Actor._
import java.lang.System


case class Cplx( val re: Double, val im: Double){
  def conj() = new Cplx( re, -im )
  def + ( z: Cplx) = new Cplx( z.re+re, z.im+im)
  def - ( z: Cplx) = new Cplx( -z.re+re, -z.im+im)
  def - () = new Cplx( -re, -im )
  def * ( z: Cplx) = new Cplx( z.re*re - z.im*im, z.re*im + z.im*re )
  def abs2() = re*re+im*im
  def * ( k: Double ) = new Cplx( re*k, im*k )
  def / ( k: Double ) = new Cplx( re/k, im/k )

  def / ( z: Cplx ):Cplx = (this*z.conj()) / z.abs2()

  override def toString() = re + "+" + im + "i"
}


object MandelCalc{
  def calc( c: Cplx ):Int = {
    var z = Cplx(0,0)
    var iter = 0;
    do{
      val abs = z.abs2()
      if (abs>4) return iter

      z = z*z+c
      iter += 1
      if (iter > 2000) return -1
    }while( true )
    iter
  }
  def calcNoCplx( c: Cplx ):Int = {
    var x,y=0.0
    var iter = 0
    do{
      val x2 = x*x
      val y2 = y*y
      if (x2+y2 > 4) return iter

      val xx = x2-y2+c.re
      y = 2*x*y+c.im
      x = xx

      iter += 1
      if (iter > 2000) return -1
    }while( true )
    iter
  }
  def calcRect( x0: Double, y0: Double, x1: Double, y1: Double, w: Int, h:Int) = {
    val out = new Array[Int]( w*h )
    val kx = (x1-x0) / w.toDouble
    val ky = (y1-y0) / w.toDouble

    for( iy <- 0 until h ; ix <- 0 until w){
      val idx = iy * w + ix 
      val z = new Cplx(ix * kx + x0, iy * ky + y0)
      out( idx ) = calc( z )
    }
    out
  }
  def calcRectNoCplx( x0: Double, y0: Double, x1: Double, y1: Double, w: Int, h:Int) = {
    val out = new Array[Int]( w*h )
    val kx = (x1-x0) / w.toDouble
    val ky = (y1-y0) / w.toDouble

    for( iy <- 0 until h ; ix <- 0 until w){
      val idx = iy * w + ix 
      val z = Cplx(ix * kx + x0, iy * ky + y0)
      out( idx ) = calcNoCplx( z )
    }
    out
  }
}

object Printer extends Actor{
  def act() = {
    loop{
      receive {
	case s:Cplx => println ("Complex: "+s)
	case s:Integer => println ("Int:"+s)
	case "stop" => exit
      }
    }
  }
}


object Upper{
  def main( args: Array[String] ) = {
/*    Printer.start()

    val z = new Cplx( 1, 1)
    Printer ! z
    val z1 = new Cplx( 2,1 )
    Printer ! z1

    Printer ! ( z / z1 )
    Printer ! "stop"
    * */
    val timeBegin1 = System.currentTimeMillis()
    //println( MandelCalc.calc( new Cplx( 0.1, 0.1 ) ));
    MandelCalc.calcRect( -2, -2, 2, 2, 2000, 2000 )
    val time1 = (System.currentTimeMillis() - timeBegin1)
    print( "Required milliseconds (with cplx):" )
    println (time1)

    val timeBegin2 = System.currentTimeMillis()
    MandelCalc.calcRectNoCplx( -2, -2, 2, 2, 2000, 2000 )
    val time2 = (System.currentTimeMillis() - timeBegin2)
    print( "Required milliseconds (no cplx):" )
    println( time2 )
  }
}
