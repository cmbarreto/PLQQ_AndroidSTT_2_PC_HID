// Esta aplicacion se conecta por bluethooth a un arduino con HID
// como las leonardo, pro micro, feather etc.
// La funcion de esta conectarse por bluethooth a la arduino y enviarle 
// por conexion serial las palabras que se le dictan al celular.
// Tambien procesa los caracteres no ascii, en este caso solamente los 
// tildes i enies del Spanish para que viajen como letras mayusculas que so
// interpretadas y ajustadas en el arduino.

// v 0.1 31-May-2020
// cmbarreto at gmail
// This work is public domain
//

var pausado = true;

//Called when application is started.
function OnStart()
{
	//Create a layout with objects vertically centered.
	lay = app.CreateLayout( "linear", "VCenter,FillXY" );	
	lay.SetBackColor( "#222222" );
	
    //Create a list view.
    lst = app.CreateList( "", 0.9, 0.5 );
    lst.SetBackColor( "black" );
    lst.SetOnTouch( lst_OnTouch );
    lay.AddChild( lst );
    
    //Get a list of paired devices into listview.
    var devices = app.GetPairedBtDevices();
	for( d in devices) 
	    lst.AddItem( devices[d].name, devices[d].address );
	
	//Create an edit with commands to send. 
    edt = app.CreateTextEdit( "Dime algo!", 0.9, 0.2, "NoSpell" );
    edt.SetMargins( 0,0.04,0,0 );
    edt.SetBackColor( "black" );
    lay.AddChild( edt ); 
    
	//Create an send button. 
    btnSend = app.CreateButton( "Pausado", 0.4, 0.1 ); 
    btnSend.SetMargins( 0,0.02,0,0 );
    btnSend.SetOnTouch( btnSend_OnTouch ); 
    lay.AddChild( btnSend ); 
  
	//Create Bluetooth serial object.
	//(also set newline as command separator)
	bt = app.CreateBluetoothSerial();
	bt.SetOnConnect( bt_OnConnect )
	bt.SetOnReceive( bt_OnReceive );
	bt.SetSplitMode( "End", "\n" );
	
	//Add main layout to app.
	app.AddLayout( lay );

    //Create recognition object and set callbacks.
	speech = app.CreateSpeechRec("NoBeep,Parxtial");
	speech.SetOnResult( speech_OnResult );
	speech.SetOnError( speech_OnError );
	
	//Say something at start (to get speech engine ready).
    app.TextToSpeech( "Pideme lo que quieras", 1, 1.5, Listen );
    //app.ShowProgress();
}

//Handle list selection.
function lst_OnTouch( title, body, type, index )
{
    app.ShowProgress( "Connecting..." );
    bt.Connect( body );
    lst.SelectItemByIndex( index );
}

//Called when we are connected.
function bt_OnConnect( ok )
{
    app.HideProgress();
    
    if( ok ) { 
        app.ShowPopup( "Connected!" ); 
        lay.RemoveChild( lst );
    }
    else { 
        app.ShowPopup( "Failed to connect!" );
        lst.SelectItemByIndex( -1 );
    }
}

//Called when user touches send button. 
function btnSend_OnTouch() 
{  
   if ( pausado )
   {
      btnSend.SetText( "Enviando");
      pausado = false;
   }
   else
   {
      btnSend.SetText( "Pausado");
      pausado = true;
   }
//   var s = edt.GetText(); 
//   bt.Write( s + " \n" );
} 

//Called when we get data from remote device.
function bt_OnReceive( data )
{
    app.ShowPopup( data );
}


//La parte de voz a texto.
//=======================


//Start recognizing.
function Listen()
{
    app.HideProgress();
	speech.Recognize();
}

//Called with the recognition result(s).
function speech_OnResult( results, partial )
{
    //Quit the function early if called with exactly the same speech rec results right away
    if (results[0] === speech.data.last_result && Date.now() - speech.data.last_result_at < 200) return;
    speech.data.last_result = results[0];
    speech.data.last_result_at = Date.now();    
    
    //Get result.
    var cmd = results[0].toLowerCase();
    //console.log( "STTCrudo " + results[0] );
    noAsciiRes = nonASCII( cmd );
    //edt.SetText(cmd + "\n" + noAsciiRes);
    edt.SetText(noAsciiRes);
    console.log( "STT: " + noAsciiRes + "\n" );
    //console.log( "BOTON: " + btnSend.GetText() + "\n" );
    if ( !pausado )
    {
      // var s = edt.GetText(); 
       bt.Write( noAsciiRes + "\ ");
      //bt.Write( noAsciiRes );
    }
    
    //Restart recognition.
    speech.Recognize();
}


//Called if recognition fails.
function speech_OnError( error )
{
    console.log( "Error" + error );
    
    //Restart recognition.
    if( !speech.IsListening() ) speech.Recognize();
}


//Process NO ASCII characters.
function nonASCII( string )
{
  str = '';
  var code,
  end = string.length,
  idx=0;

  while (idx < end) {
     code = string.charCodeAt(idx);
        switch (code) {
          case 225:
            str = str + "T" + "a";
            break;
          case 233:
            str = str + "T" + "e";
            break;
          case 237:
            str = str + "T" + "i";
            break;
          case 243:
            str = str + "T" + "o";
            break;
          case 250:
            str = str + "T" + "u";
            break;
          case 241:
            str = str + "N";
            break;

          default:
            str = str + string.charAt(idx);
        }
     idx++;
     //console.log( "Codigo " + code );
  }
  return str;
}
