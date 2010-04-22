import javax.swing._
import java.awt._
import java.awt.event._
import java.lang.System

class Point( x: Int, y: Int ){
  var xx = x*x
  override def toString() = {
    "("+x+","+y+")"
  }
}


class MyPaintBox( paint_func: Graphics=>Unit ) extends JComponent{
  override def paintComponent( g: Graphics ) = {
    paint_func( g )
  }
}

class Matrix(h:Int, w:Int){
  val data = new Array[Float]( h*w )
  def size() = (h,w)
  def get(i:Int, j:Int) = data(i+j*h)
  def set(i:Int, j:Int, v:Float) = {data(i+j*h) = v}
  def height() = h
  def width() = w
  def setMul( a:Matrix, b:Matrix){
    //this <- a*b
    if (a.height != height || b.width != width)
      error( "A or B size don't match C size" )
    if (a.width != b.height )
      error( "A and B sizes don't match" )

    var idx = 0;
    for ( i <- 0 until h; j <- 0 until  w){
      var s: Float = 0
      for( k <- 0 until a.width ){
	s += a.get(i, k) * b.get( k, j )
      }
      data( idx ) = s
      idx += 1
    }
  }
}

object Timer{
  def run( runnable: ()=>Unit ) = {
    val timeBegin = System.currentTimeMillis
    runnable()
    System.currentTimeMillis - timeBegin
  }
  def runUntil( runnable: ()=>Unit, minTimeMs: Long ) = {
    var elapsed:Long = 0
    var iters = 0
    while( elapsed < minTimeMs ){
      elapsed += run( runnable )
      iters += 1
    }
    (elapsed, iters)
  }
}



object MainClass{

  def main(args: Array[String]) = {
    println ("Learning...")
    var p = new Point( 10, 20 )
    println( p )
    println( p.xx )

    var m = new Matrix( 4,4 )
    println( m )


    //measuringTime
    println( Timer.runUntil( ()=> {
      var m = new Matrix( 10,10)
      m.setMul( m, m)
    }, 10000 ))


    val frm = new JFrame( "test frame" )
    frm.setSize( new Dimension( 256, 256 ) )
    frm.setDefaultCloseOperation( JFrame.EXIT_ON_CLOSE )
    
    frm.addWindowListener( new WindowAdapter{
      override def windowClosing( evt:WindowEvent ) = {
	println("WindowClosing event")
      }
    })

   

    val comp = new MyPaintBox( _.drawLine( 0,0, 10, 10) )

    frm.getContentPane.add( comp )


    frm.setVisible( true )
    println("Waiting for window close")
    
    
  }
}
