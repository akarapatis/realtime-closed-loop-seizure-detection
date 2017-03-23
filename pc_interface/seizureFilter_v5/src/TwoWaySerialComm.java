import gnu.io.CommPort;
import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Enumeration;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.jfree.data.time.Millisecond;

public class TwoWaySerialComm
{
    SerialReader fromDevice;
    SerialWriter toDevice;
    SerialPort serialPort;
    PrintWriter outputfile;
    boolean readingDone;
    
    
    public TwoWaySerialComm()
    {
        super();
        readingDone = false;
    }
    
    
    public void openFile(){
        DateFormat dateFormat = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");
            Calendar cal = Calendar.getInstance();
             
            try {
                outputfile = new PrintWriter("filter_output_"+String.format("%1$tm%1$td%1$tH%1$tM%1$tS", cal)  +".txt");
                outputfile.format("%% Start of logging :" + dateFormat.format(cal.getTime()) + "\n") ;
                outputfile.format("%% Input   FilterOUT  \n") ;
                
            } catch (Exception e) {
                System.out.println("File could not be created " + e.toString());
                outputfile = null;
            }
    }
    
    public void closeFile(){
        if(this.outputfile!=null)
            this.outputfile.close();
        
        this.outputfile = null;
    }
    
    void connect ( String portName ) throws Exception
    {
        CommPortIdentifier portIdentifier = CommPortIdentifier.getPortIdentifier(portName);
        if ( portIdentifier.isCurrentlyOwned() )
        {
            System.out.println("Error: Port is currently in use");
        }
        else
        {
            CommPort commPort = portIdentifier.open(this.getClass().getName(),2000);
            
            
            if ( commPort instanceof SerialPort )
            {
                serialPort = (SerialPort) commPort;
                serialPort.setSerialPortParams(115200,SerialPort.DATABITS_8,SerialPort.STOPBITS_1,SerialPort.PARITY_NONE);
                
                InputStream in = serialPort.getInputStream();
                OutputStream out = serialPort.getOutputStream();
                
                fromDevice = new SerialReader(in, this);
                (new Thread(fromDevice)).start();
                toDevice = new SerialWriter(out);
                (new Thread(toDevice)).start();
                
                try {
                    toDevice.out.write('a');
                    toDevice.out.write('\r');

                    Thread.sleep(100);

                    toDevice.out.write('g');
                    toDevice.out.write('\r');
                  
                    
                    Thread.sleep(400);

                } catch (IOException e) {
                    e.printStackTrace();
                } catch (InterruptedException ex) {
                    Logger.getLogger(TwoWaySerialComm.class.getName()).log(Level.SEVERE, null, ex);
                }

                
            }
            else
            {
                System.out.println("Error: Only serial ports are handled by this example.");
            }
        }     
    }
    
    
    synchronized boolean doneStatus(boolean value, int type){
        
        if( type  == 1){
            readingDone = value;
            return true;
        }
        else
            return readingDone;
        
    } 
    
    @SuppressWarnings("empty-statement")
    void close(boolean dataRxON){
                
        fromDevice.stop = true;
        
        System.out.println("Waiting reader to finish");
//        while(doneStatus(false,0) == false ){}
        System.out.println("Reader done from parent");
        
       
            try {

                toDevice.out.write('a');
                toDevice.out.write('\r');

                Thread.sleep(100);

                toDevice.out.write('g');
                toDevice.out.write('\r');

                Thread.sleep(400);
                
                 if (!dataRxON) {
                    toDevice.out.write('a');
                    toDevice.out.write('\r');

                    Thread.sleep(100);

                    toDevice.out.write('g');
                    toDevice.out.write('\r');
                }

            } catch (IOException e) {
                e.printStackTrace();
            } catch (InterruptedException ex) {
                Logger.getLogger(TwoWaySerialComm.class.getName()).log(Level.SEVERE, null, ex);
            }
           
        serialPort.close();
        this.closeFile();
        
        
    }

    
    public static class SerialReader implements Runnable 
    {
        InputStream in;
        boolean stop;
        boolean isDone;
        TwoWaySerialComm parent;
        
        static int TELEMDATA_SIZE = 11;
        static int DETECTMSG_SIZE = 29;
        
        public SerialReader ( InputStream in ,TwoWaySerialComm parent)
        {
            this.in = in;
            
            stop = false;
            isDone = false;
            
            this.parent = parent;
            
        }
        
        
        boolean checkVal( byte  c){
            
            if (c == 13)
                return true;
            
            if (c == 10)
                return true;
            
            return c>=20 && c<=126;           
            
        }
        
        @Override
        public void run ()
        {
            byte[] buffer = new byte[4096];
            byte[] telemetryData = new byte[4096];
            
            boolean telemetryIn = false;
            boolean telemetryRead = false;
            
            int counterData = 0;
            int counterMSG  = 0;
            int len = -1;
            
            long start,end,avg=0;
            long waitStart=0, waitEnd=0;
            long procStart=0, procEnd=0;
            long maxWait=0;
            long maxProc=0;
            
            long overTimeS=0 ,overTimeF=0;
            long overTimeCnt=0;        
            
            
            int timeCounter=0;
            long max = 0;
            
            try
            {
                while (!this.stop && (counterData >= TELEMDATA_SIZE || ( len = this.in.read(buffer)) > -1) )
                { 
                    waitEnd = System.nanoTime();
                    start = System.nanoTime();
                    
                    // Calculate waiting time for data, and update max Value
                    if(waitStart!=0 && maxWait < waitEnd-waitStart){
                        maxWait = waitEnd-waitStart;
                    }
                    
                    
                    if(counterData < 0){
                        System.out.println("CounterData cannot be negative ");
                        counterData = 0;
                    }
                     
                    
                    // 1st byte is IN
                    if(buffer[0] == -56 && telemetryIn == false){
                        telemetryIn = true;
                        
                        //Reset counting 
                        counterData = 0;
                        counterMSG = 0;
                    }                    
                    
                    
                    // Telemetry is available
                    if( telemetryIn == true || counterData !=0 ){
                        
                        
                        ////////////////////////////////////////////////////////
                        if(len>1024){
                           System.out.println("CounterData"+ counterData +" len " + len + " too big ");
                           counterData = 0; 
                           telemetryIn = false;
                           continue;
                        }
                        
                        if(counterData>=(1024-len) ){
                            System.out.println("CounterData"+ counterData +" too big , len: " + len);
                            counterData = 0; 
                            telemetryIn = false;
                            continue;
                        }
                        ///////////////////////////////////////////////////////
                        
                        if(len != 0){
                            //Copy Data to telemetryFrame
                            try{
                                System.arraycopy(buffer, 0, telemetryData, counterData, len);
                                counterData += len;
                            }
                            catch(ArrayIndexOutOfBoundsException e){
                                System.out.println("Error: "+ buffer);
                                System.out.println("Len  : "+len);
                                System.out.println("counterData: "+counterData);
                                counterData =0;
                                telemetryIn = false;        
                            }
                        }
                        
                        
                        // Check how many frames avail or messages
                        while(counterData >= TELEMDATA_SIZE){
                            telemetryRead = true;
                            
                            if( telemetryData[0] == -56 && telemetryData[TELEMDATA_SIZE-1] == '\n'){
                                procStart = System.nanoTime();
                                processData(telemetryData);
                                procEnd = System.nanoTime();
                                
                                if(maxProc < procEnd - procStart)
                                    maxProc = procEnd - procStart;
                                
                                
                                counterData -= TELEMDATA_SIZE;
                                for (int i = 0; i < counterData; i++) {
                                    telemetryData[i] = telemetryData[TELEMDATA_SIZE+i];
                                }
                            }
                            else if ( counterData >= DETECTMSG_SIZE){
                                System.out.print(new String(telemetryData,0,DETECTMSG_SIZE));
                                counterData -= DETECTMSG_SIZE;
                                for (int i = 0; i < counterData; i++) {
                                    telemetryData[i] = telemetryData[DETECTMSG_SIZE+i];
                                }                                
                            }
                            else{
                                // No complete frame, or message available
                                break;
                            }
                        }
                        
                        
                        if(counterData > 0 && telemetryRead == true){
                            telemetryRead = false;
                            
                            // Next data has arrived
                            if(telemetryData[0] == -56){
                                telemetryIn = true;
                                counterMSG = 0;
                            }
                            else{
                                
                                //Data needs to be printed
                                //System.out.println("BDG :{" + new String(buffer,0,len) + "} len is :"+ len + "counter is: "+ counterData);
                                System.out.print(new String(telemetryData,0,counterData));
                                
                                counterMSG += counterData;
                                counterData = 0;
                                telemetryIn = false;
                                
                            }
                        }
                        
                        else if(counterData == 0 ){
                            counterMSG    = 0 ;
                            telemetryIn   = false;
                            telemetryRead = false;
                        }
                        
                    } else {
                        
                        counterMSG += len;
                        
                        boolean telemFOUND = false;
                        int     telem_pos=0;
                        
                        
                        if( counterMSG > DETECTMSG_SIZE){
                            telem_pos = 0;
                            
                            
                            for (; telem_pos < len ; telem_pos++) {
                                if( buffer[telem_pos] == -56){
                                    telemFOUND = true;
                                    break;
                                }
                            }
                        
                        }
                        
                        if(telemFOUND == true){
                             for (int i = 0; i < len-telem_pos; i++) {
                                telemetryData[i] = buffer[i+telem_pos];
                            }
                            counterData = len - telem_pos;
                            telemetryIn = true;
                            counterMSG  = 0;
                            telemetryRead = false;
                            
                        }
                        else{
                            telem_pos = 0;
                        }
                         
                        if(telemFOUND == true){
                            System.out.print(new String(buffer,0,telem_pos));

                        }
                        else{
  
                            System.out.print(new String(buffer,0,len));
                            
                            counterData = 0;
                            telemetryIn = false;
                        }
                        
                        
                    }
                    
                    
                    len = 0;
                    end = System.nanoTime();
                    avg = (avg + (end-start))/2;
                    
                     if(max < end-start)
                            max = end-start;
                    
                     if(timeCounter == 100){
                        //System.out.format("max:%9.2f | wait %8.4f | proc: %8.4f Series %d\n", max/1000000.0, maxWait/1000000.0, maxProc/1000000.0 , PlotGUI.qr.seriesQ.getItemCount());                        
                        max=0;
                        maxWait = 0;
                        maxProc = 0;
                        timeCounter=0;
                        avg=0;
                        
                    }
                    timeCounter++;
                    
                    /* Check if time crosses 20 ms */
                    if( end-start > 20000000){
                        if(overTimeCnt == 0){
                            overTimeS = System.nanoTime();
                            overTimeCnt++;
                        }else if( (System.nanoTime() - overTimeS)> 200000000 ){
                            overTimeCnt=0;
                        }else{
                            overTimeCnt++;
                        }
                        
                        if(overTimeCnt > 4){
                            overTimeCnt=0;
                            System.out.println("Loop Time:"+ (end-start)/1000 + "us| Data Wait: " + (waitEnd-waitStart)/1000 + "us| Proc is: " + maxProc);
                        }
                        
                    }
                    
                    waitStart = System.nanoTime();
                    
                }
            }
            catch ( IOException e )
            {
                if( stop != true)
                    e.printStackTrace();
            } 
            
            parent.doneStatus(true, 1);
            parent.doneStatus(true, 0);
            
        }
        
        
        public void processData(byte[] buffer) throws IOException{
            int input=0;
            int filterOut=0;
            int []shift = {24,16,8,0};
            long now;
            
            
            for (int i = 1; i < 5; i++) {
                input |=  (((int)(buffer[i])) << shift[i-1]) & (0x000000FF << shift[i-1]);
            }      
            
            
            for (int i = 5; i < 9; i++) {
                filterOut |=  (((int)(buffer[i])) << shift[i-5]) & (0x000000FF << shift[i-5]);
            }
            
            if(parent.outputfile != null)
               parent.outputfile.printf("%d %d \n",input, filterOut);
            
            now = new Millisecond().getFirstMillisecond();
            PlotGUI.controlGUI.seriesQ.add(now, (float)(input * 1.8/4096.0));
            PlotGUI.controlGUI.seriesP.add(now, (float)filterOut/1000);
                        
        }
    }
    
    void toggleGUI() {
        try {
            
            toDevice.out.write('g');
            toDevice.out.write('\r');
            
            Thread.sleep(100);
            
            toDevice.out.write('a');
            toDevice.out.write('\r');
            
            
        } catch (IOException e) {
             e.printStackTrace();
        } catch (InterruptedException ex) {
            Logger.getLogger(TwoWaySerialComm.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    void selectMonChannel(int chan){
       
        try {
            
            toDevice.out.write('d');
            toDevice.out.write('\r');
            
            toDevice.out.write((Integer.toString(chan)).getBytes());
            toDevice.out.write('\r');
            
        } catch (IOException e) {
             e.printStackTrace();
        }
    }
    
    void confChannels(byte chanMask, int[] upper, int[] lower){
        
        try {
            
            toDevice.out.write('c');
            toDevice.out.write('\r');            
           
            toDevice.out.write((Integer.toString(chanMask)).getBytes());
            toDevice.out.write('\r');
            
        } catch (IOException e) {
             e.printStackTrace();
        }
        
        
        try {
            
            toDevice.out.write('t');
            toDevice.out.write('\r');            
            
            toDevice.out.write((Integer.toString(chanMask)).getBytes());
            toDevice.out.write('\r');
            
            for (int i = 0; i < 4; i++) {
                System.out.println("Chan "+ i + " upper: " + upper[i] + " lower: " + lower[i]);
                
                if( (chanMask & 1<<(i)) != 0 ){
                    toDevice.out.write((Integer.toString(upper[i])).getBytes());
                    toDevice.out.write('\r');
                    toDevice.out.write((Integer.toString(lower[i])).getBytes());
                    toDevice.out.write('\r');
                }
                
            }
            
          
            
        } catch (IOException e) {
             e.printStackTrace();
        }
        
    }
    
    void setLed(int delay, int ontime, int locktime){
          System.out.println("Set led delay:"+ delay + " on:"+ ontime + " lock:"+ locktime);
          try {
            
            toDevice.out.write('l');
            toDevice.out.write('\r');            
           
            toDevice.out.write((Integer.toString(delay)).getBytes());
            toDevice.out.write('\r');
            toDevice.out.write((Integer.toString(ontime)).getBytes());
            toDevice.out.write('\r');
            toDevice.out.write((Integer.toString(locktime)).getBytes());
            toDevice.out.write('\r');
            
        } catch (IOException e) {
             e.printStackTrace();
        }
    }
            
    void turnLedOn(){
        try {
            
            toDevice.out.write('z');
            toDevice.out.write('\r');
            
        } catch (IOException e) {
             e.printStackTrace();
        }
        
    }
    
    
    public static String[] listSerialPorts() {

        Enumeration ports = CommPortIdentifier.getPortIdentifiers();
        ArrayList portList = new ArrayList();
        String portArray[] = null;
        while (ports.hasMoreElements()) {
            CommPortIdentifier port = (CommPortIdentifier) ports.nextElement();
            if (port.getPortType() == CommPortIdentifier.PORT_SERIAL) {
                portList.add(port.getName());
            }
        }
        portArray = (String[]) portList.toArray(new String[0]);
        return portArray;
    }

    public static class SerialWriter implements Runnable 
    {
        OutputStream out;
        boolean stop;
        boolean isDone;
        
        public SerialWriter ( OutputStream out )
        {
            this.out = out;
            stop = false;
            isDone = false;
        }
        
        public void run ()
        {
            try
            {                
                int c = 0;
                
                while ( c > -1 )
                {
                    
                    c = System.in.read();
                    
                    if(c==10){
                        
                        this.out.write(13);
                    }
                    else
                        this.out.write(c);
                    
                }                
            }
            catch ( IOException e )
            {
                e.printStackTrace();
            }
            
        }
    }
    
}
