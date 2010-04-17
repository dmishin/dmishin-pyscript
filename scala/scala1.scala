import scala.actors._
import scala.actors.Actor._

class Cplx( val re: Double, val im: Double){
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
  def calc( c: Cplx ):Int{
    var z = new Cplx(0,0)
    var iter = 0

    do{
      val abs = z.abs2()
      if (abs>4) return iter
      z = z*z+c
      iter += 1
      if (iter > 2000) return -1
    }while( true )
  }

}

object Printer extends Actor{
  def act(){
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
    println( MandelCalc.calc( new Cplx( 0.5, 0.5 ) ));
  }
}
