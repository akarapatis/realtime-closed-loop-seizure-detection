
/**
 * @author Thanas Karapatis
 * @version 4
 *
 * Difference of this version: + Scale(zoom out) time + Configure Thresholds and
 * enable channels + Turn LED on
 *
 *
 */

import gnu.io.CommPort;
import gnu.io.CommPortIdentifier;
import gnu.io.PortInUseException;
import java.awt.Color;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.text.SimpleDateFormat;
import java.util.Enumeration;
import java.util.HashSet;
import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.border.TitledBorder;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.DateAxis;
import org.jfree.chart.axis.ValueAxis;
import org.jfree.chart.plot.ValueMarker;
import org.jfree.chart.plot.XYPlot;
import org.jfree.data.Range;
import org.jfree.data.xy.XYDataset;

@SuppressWarnings("serial")
public class PlotGUI extends JFrame implements ActionListener {

    static PlotGUI controlGUI;
    TwoWaySerialComm beagle;

    final int dimX = 400;
    final int dimY = 700;

    XYSeriesNew seriesP = new XYSeriesNew("Output");
    XYSeriesNew seriesQ = new XYSeriesNew("Input");

    JButton fileRecord;
    
    JTextField upperChan0;
    JLabel upperChan0Lbl;
    
    JTextField upperChan1;
    JLabel upperChan1Lbl;
    
    JTextField upperChan2;
    JLabel upperChan2Lbl;
    
    JTextField upperChan3;
    JLabel upperChan3Lbl;
    
    JTextField lowerChan0;
    JLabel lowerChan0Lbl;
    
    JTextField lowerChan1;
    JLabel lowerChan1Lbl;
    
    JTextField lowerChan2;
    JLabel lowerChan2Lbl;
    
    JTextField lowerChan3;
    JLabel lowerChan3Lbl;
    
    JCheckBox chan0En;
    JCheckBox chan1En;
    JCheckBox chan2En;
    JCheckBox chan3En;

    JFreeChart chart1;
    JFreeChart chart2;
    
    private JLabel ledDelayLbl;
    private JTextField ledDelay;
    private JLabel ledONLbl;
    private JTextField ledON;
    private JLabel ledLockLbl;
    private JTextField ledLock;
    private JComboBox channelCombo;
    private ValueMarker markerThres;
    
    
    byte chanSelection=1;
    int []upperChanVals;
    int []lowerChanVals;
    private boolean dataRxON;

    /**
     * The time series data.
     */
    @SuppressWarnings("deprecation")
    public PlotGUI() {

        super("Seizure monitor");
        setTitle("Seizure monitor");
        
        chanSelection = 1;
        dataRxON = true;
        upperChanVals = new int[4];
        lowerChanVals = new int[4];
        
        // Container of window
        Container contentPanel = this.getContentPane();
        contentPanel.setLayout(new GridBagLayout());
        GridBagConstraints c = new GridBagConstraints();
        
        // Control Panel
        JPanel controlPanel = new JPanel();
        controlPanel.setLayout(new BoxLayout(controlPanel,
        BoxLayout.Y_AXIS));
        controlPanel.setPreferredSize(new Dimension(320, this.dimY));
        createControlPanel(controlPanel);
        
        c.fill = GridBagConstraints.VERTICAL;
        c.weightx = 0.0;
        c.gridx = 0;
        c.gridy = 0;
        contentPanel.add(controlPanel, c);
        
        // Chart Pane
        final JPanel chartPane = new JPanel(new GridLayout(2, 1));
        chartPane.setPreferredSize(new Dimension(800, this.dimY));
        createChartPanel(chartPane);
        
        c.fill = GridBagConstraints.BOTH;
        c.weightx = 1.0;
        c.weighty = 1.0;
        c.gridx = 1;
        c.gridy = 0;
        contentPanel.add(chartPane, c);

    }

    private JFreeChart createChart(final XYDataset dataset, double min,
            double max, String title, boolean autorange) {
        final JFreeChart result;
        final XYPlot plot;
        if(autorange){
             result = ChartFactory.createTimeSeriesChart(title,
                "Time", "Output ( x1000 )", dataset, true, true, false);
             
            plot = result.getXYPlot();
            ValueAxis axisY = plot.getRangeAxis();
            
            axisY.setRange(new Range(min, 10000),false,false);
            
        }
        else{
             result = ChartFactory.createTimeSeriesChart(title,
                "Time", "Voltage", dataset, true, true, false);
            plot = result.getXYPlot();
            ValueAxis axisY = plot.getRangeAxis();
            axisY.setRange(new Range(min, max));
          
            
        }
        

        DateAxis axisX = (DateAxis) plot.getDomainAxis();
        axisX.setFixedAutoRange(50000.0); // 50 seconds
        axisX.setDateFormatOverride(new SimpleDateFormat("ss.SS"));

        

        return result;
    }

    public static String[] getAvailableSerialPorts() {
        HashSet<String> h = new HashSet<>();
        Enumeration thePorts = CommPortIdentifier.getPortIdentifiers();
        while (thePorts.hasMoreElements()) {
            CommPortIdentifier com = (CommPortIdentifier) thePorts.nextElement();
            switch (com.getPortType()) {
                case CommPortIdentifier.PORT_SERIAL:
                    try {
                        CommPort thePort = com.open("CommUtil", 50);
                        thePort.close();
                        h.add(com.getName());
                    } catch (PortInUseException e) {
                        System.out.println("Port, " + com.getName() + ", is in use.");
                    } catch (Exception e) {
                        System.err.println("Failed to open port " + com.getName());
                        e.printStackTrace();
                    }
            }
        }

        return h.toArray(new String[h.size()]);
    }

    void createGUI(String port) {

        try {

            beagle = (new TwoWaySerialComm());
            beagle.connect(port);
            controlGUI.addWindowListener(new WindowAdapter() {
                @Override
                public void windowClosing(WindowEvent ev) {
                    dispose();
                    beagle.close(dataRxON);
                    System.exit(0);
                }
            });

        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
            System.exit(1);
        }

        controlGUI.setSize(controlGUI.dimX, controlGUI.dimY);
        controlGUI.pack();
        controlGUI.setLocationRelativeTo(null);
        controlGUI.setVisible(true);

    }

    /**
     *
     * @param args
     *
     */
    public static void main(String[] args) {
        String[] ports;
        // build GUI
        controlGUI = new PlotGUI();
        PortSelection diag = new PortSelection(controlGUI, getAvailableSerialPorts());

    }
        
    @Override
    public void actionPerformed(ActionEvent e) {

        if (e.getActionCommand().equals("StartRecord")) {
            fileRecord.setText("Stop LOG");
            fileRecord.setActionCommand("StopRecord");
            beagle.openFile();
        } else if (e.getActionCommand().equals("StopRecord")) {
            fileRecord.setText("Start LOG");
            fileRecord.setActionCommand("StartRecord");
            beagle.closeFile();
        } else if (e.getActionCommand().equals("chan0En")) {
            if(chan0En.isSelected()){
                upperChan0Lbl.setEnabled(true);
                upperChan0.setEnabled(true);
                lowerChan0Lbl.setEnabled(true);
                lowerChan0.setEnabled(true);
                chanSelection |= 1<<0;
                
            } else {
                upperChan0Lbl.setEnabled(false);
                upperChan0.setEnabled(false);
                lowerChan0Lbl.setEnabled(false);
                lowerChan0.setEnabled(false);
                chanSelection &= ~((byte)(1<<0));
            }
            
        } else if (e.getActionCommand().equals("chan1En")) {
            if(chan1En.isSelected()){
                upperChan1Lbl.setEnabled(true);
                upperChan1.setEnabled(true);
                lowerChan1Lbl.setEnabled(true);
                lowerChan1.setEnabled(true);
                chanSelection |= 1<<1;
                
            } else {
                upperChan1Lbl.setEnabled(false);
                upperChan1.setEnabled(false);
                lowerChan1Lbl.setEnabled(false);
                lowerChan1.setEnabled(false);
                chanSelection &= ~((byte)(1<<1));
            }
        } else if (e.getActionCommand().equals("chan2En")) {
            if(chan2En.isSelected()){
                upperChan2Lbl.setEnabled(true);
                upperChan2.setEnabled(true);
                lowerChan2Lbl.setEnabled(true);
                lowerChan2.setEnabled(true);
                chanSelection |= 1<<2;
                
            } else {
                upperChan2Lbl.setEnabled(false);
                upperChan2.setEnabled(false);
                lowerChan2Lbl.setEnabled(false);
                lowerChan2.setEnabled(false);
                chanSelection &= ~((byte)(1<<2));
                
            }
        } else if (e.getActionCommand().equals("chan3En")) {
            if(chan3En.isSelected()){
                upperChan3Lbl.setEnabled(true);
                upperChan3.setEnabled(true);
                lowerChan3Lbl.setEnabled(true);
                lowerChan3.setEnabled(true);
                chanSelection |= (1<<3);
                
            } else {
                upperChan3Lbl.setEnabled(false);
                upperChan3.setEnabled(false);
                lowerChan3Lbl.setEnabled(false);
                lowerChan3.setEnabled(false);
                chanSelection &= ~((byte)(1<<3));
            }
        } else if (e.getActionCommand().equals("monitorList")) {
            // TODO: Add selection
            int selectedMon = channelCombo.getSelectedIndex();
            beagle.selectMonChannel( selectedMon );
            
            switch (selectedMon){
                case 0: 
                    if(chan0En.isSelected()){
                        
                        XYPlot plot = (XYPlot) chart1.getPlot();
                        if(markerThres!= null){
                            plot.removeRangeMarker(markerThres);
                        }
                        
                        markerThres = new ValueMarker(Double.parseDouble(upperChan0.getText()));  // position is the value on the axis
                        markerThres.setPaint(Color.GREEN);
                        plot.addRangeMarker(markerThres);
                        
                    }
                    break;
                case 1:
                    if(chan1En.isSelected()){
                        
                        XYPlot plot = (XYPlot) chart1.getPlot();
                        if(markerThres!= null){
                            plot.removeRangeMarker(markerThres);
                        }
                        
                        markerThres = new ValueMarker(Double.parseDouble(upperChan1.getText()));  // position is the value on the axis
                        markerThres.setPaint(Color.GREEN);
                        plot.addRangeMarker(markerThres);
                        
                    }
                    break;
                case 2:
                    if(chan2En.isSelected()){
                        
                        XYPlot plot = (XYPlot) chart1.getPlot();
                        if(markerThres!= null){
                            plot.removeRangeMarker(markerThres);
                        }
                        
                        markerThres = new ValueMarker(Double.parseDouble(upperChan2.getText()));  // position is the value on the axis
                        markerThres.setPaint(Color.GREEN);
                        plot.addRangeMarker(markerThres);
                        
                    }
                    break;
                case 3:
                    if(chan3En.isSelected()){
                        
                        XYPlot plot = (XYPlot) chart1.getPlot();
                        if(markerThres!= null){
                            plot.removeRangeMarker(markerThres);
                        }
                        
                        markerThres = new ValueMarker(Double.parseDouble(upperChan3.getText()));  // position is the value on the axis
                        markerThres.setPaint(Color.GREEN);
                        plot.addRangeMarker(markerThres);
                        
                    }
                    break;
                default:
                    break;
            }
            
        }else if (e.getActionCommand().equals("testLed")) {
            beagle.turnLedOn();
        }
        else if (e.getActionCommand().equals("setLed")) {
            int delay = Integer.parseInt(ledDelay.getText());
            int ontime = Integer.parseInt(ledON.getText());
            int locktime = Integer.parseInt(ledLock.getText());
                
            beagle.setLed(delay, ontime, locktime);
        }
        else if (e.getActionCommand().equals("confButton")) {
            int upperVal, lowerVal;
            
            if( (chanSelection&1) != 0 ) {
                if(upperChan0.getText().equals("") ){
                    upperVal = 0;
                } else{
                    upperVal = Integer.parseInt(upperChan0.getText());
                }
                
                if(lowerChan0.getText().equals("") ){
                    lowerVal = 0;
                } else{
                    lowerVal = Integer.parseInt(lowerChan0.getText());
                }
                
                upperChanVals[0] = upperVal;
                lowerChanVals[0] = lowerVal;
                 
            }
             
            if ((chanSelection & 2) != 0){
                if(upperChan1.getText().equals("") ){
                    upperVal = 0;
                } else{
                    upperVal = Integer.parseInt(upperChan1.getText());
                }
                
                if(lowerChan1.getText().equals("") ){
                    lowerVal = 0;
                } else{
                    lowerVal = Integer.parseInt(lowerChan1.getText());
                }
                
                upperChanVals[1] = upperVal;
                lowerChanVals[1] = lowerVal;
            }
            
            if ((chanSelection & 4) != 0){
                if(upperChan2.getText().equals("") ){
                    upperVal = 0;
                } else{
                    upperVal = Integer.parseInt(upperChan2.getText());
                }
                
                if(lowerChan2.getText().equals("") ){
                    lowerVal = 0;
                } else{
                    lowerVal = Integer.parseInt(lowerChan2.getText());
                }
                
                upperChanVals[2] = upperVal;
                lowerChanVals[2] = lowerVal;
            }
            
            if ((chanSelection & 8) != 0) {
                if(upperChan3.getText().equals("") ){
                    upperVal = 0;
                } else{
                    upperVal = Integer.parseInt(upperChan3.getText());
                }
                
                if(lowerChan3.getText().equals("") ){
                    lowerVal = 0;
                } else{
                    lowerVal = Integer.parseInt(lowerChan3.getText());
                }
                
                upperChanVals[3] = upperVal;
                lowerChanVals[3] = lowerVal;
            }
            
            beagle.confChannels(chanSelection, upperChanVals, lowerChanVals);
        }else if (e.getActionCommand().equals("toggle")) {
            beagle.toggleGUI();
            dataRxON = !dataRxON;
        }
        
        

    }

    /**
     * Create controls for GUI
     *
     * @param controlPanel JPanel of control menu
     */
    final void createControlPanel(JComponent controlPanel) {
        
        fileRecord = new JButton("Start LOG");
        fileRecord.setActionCommand("StartRecord");
        
        JButton toggleGUI = new JButton("Toggle GUI");
        toggleGUI.setActionCommand("toggle");
        toggleGUI.addActionListener(this);
        
        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new FlowLayout(FlowLayout.CENTER));
        buttonPanel.add(fileRecord);
        buttonPanel.add(toggleGUI);
        
        // ---------------------------------------------------------------------
        //                  Monitor Channel
        // ---------------------------------------------------------------------
        TitledBorder titled = BorderFactory.createTitledBorder("Monitor Channel");
        
        String[] channelsMon = {"1", "2", "3", "4"};
        channelCombo = new JComboBox(channelsMon);
        channelCombo.setSelectedIndex(0);
        channelCombo.setPreferredSize(new Dimension(100, 30));
        channelCombo.setActionCommand("monitorList");
        channelCombo.addActionListener(this);
        
        JPanel comp = new JPanel(new FlowLayout(FlowLayout.CENTER));
        comp.add(channelCombo);
        comp.setBorder(titled);
        
        // ---------------------------------------------------------------------
        //                  TOP PANNEL
        // ---------------------------------------------------------------------
        JPanel topPanel = new JPanel();
        topPanel.setLayout(new GridLayout(1, 2));
        topPanel.setPreferredSize(new Dimension(300, 80));
        topPanel.setMaximumSize(new Dimension(300, 80));
        topPanel.add(buttonPanel);
        topPanel.add(comp);
        
        
        
        // Channels panel
        JPanel channelsPane = new JPanel(new GridLayout(2,2));
        // ---------------------------------------------------------------------
        //                  Channel 1
        // ---------------------------------------------------------------------
        titled = BorderFactory.createTitledBorder("Channel 1");
        chan0En = new JCheckBox("Enabled");
        chan0En.addActionListener(this);
        chan0En.setActionCommand("chan0En");
        chan0En.setSelected(false);
        upperChan0 = new JTextField("25000");
        lowerChan0 = new JTextField("1000");
        
        comp = new JPanel(new GridLayout(5, 1, 2, 2), false);
        comp.setPreferredSize(new Dimension(150, 120));
        
        comp.add(chan0En);
        comp.add(upperChan0Lbl = new JLabel("Upper Threshold"));
        comp.add(upperChan0);
        comp.add(lowerChan0Lbl = new JLabel("Lower Threshold"));
        comp.add(lowerChan0);
        comp.setBorder(titled);
        
        upperChan0Lbl.setEnabled(false);
        upperChan0.setEnabled(false);
        lowerChan0Lbl.setEnabled(false);
        lowerChan0.setEnabled(false);

        
        channelsPane.add(comp);
        
        // ---------------------------------------------------------------------
        //                  Channel 2
        // ---------------------------------------------------------------------
        titled = BorderFactory.createTitledBorder("Channel 2");
        comp = new JPanel(new GridLayout(5, 1, 2, 2), false);
        comp.setPreferredSize(new Dimension(150, 120));
        comp.setBorder(titled);
        
        chan1En = new JCheckBox("Enabled");
        chan1En.addActionListener(this);
        chan1En.setActionCommand("chan1En");
        
        upperChan1Lbl =new JLabel("Upper Threshold");
        upperChan1 = new JTextField("25000");
        lowerChan1Lbl = new JLabel("Lower Threshold");
        lowerChan1 = new JTextField("1000");
        
        upperChan1Lbl.setEnabled(false);
        upperChan1.setEnabled(false);
        lowerChan1Lbl.setEnabled(false);
        lowerChan1.setEnabled(false);

        comp.add(chan1En);
        comp.add(upperChan1Lbl);
        comp.add(upperChan1);
        comp.add(lowerChan1Lbl);
        comp.add(lowerChan1);
        
        
        channelsPane.add(comp);
        
        // ---------------------------------------------------------------------
        //                  Channel 3
        // ---------------------------------------------------------------------
        titled = BorderFactory.createTitledBorder("Channel 3");
        comp = new JPanel(new GridLayout(5, 1, 2, 2), false);
        comp.setPreferredSize(new Dimension(150, 120));
        comp.setBorder(titled);
        
        chan2En = new JCheckBox("Enabled");
        chan2En.addActionListener(this);
        chan2En.setActionCommand("chan2En");
        upperChan2Lbl = new JLabel("Upper Threshold");
        upperChan2 = new JTextField("25000");
        lowerChan2Lbl = new JLabel("Lower Threshold");
        lowerChan2 = new JTextField("1000");
        
        upperChan2Lbl.setEnabled(false);
        upperChan2.setEnabled(false);
        lowerChan2Lbl.setEnabled(false);
        lowerChan2.setEnabled(false);

        comp.add(chan2En);
        comp.add(upperChan2Lbl);
        comp.add(upperChan2);
        comp.add(lowerChan2Lbl);
        comp.add(lowerChan2);
        channelsPane.add(comp);

        
        // ---------------------------------------------------------------------
        //                  Channel 4
        // ---------------------------------------------------------------------
        titled = BorderFactory.createTitledBorder("Channel 4");
        comp = new JPanel(new GridLayout(5, 1, 2, 2), false);
        comp.setPreferredSize(new Dimension(150, 120));
        comp.setBorder(titled);
        
        chan3En = new JCheckBox("Enabled");
        chan3En.addActionListener(this);
        chan3En.setActionCommand("chan3En");
        upperChan3Lbl = new JLabel("Upper Threshold");
        upperChan3 = new JTextField("25000");
        lowerChan3Lbl = new JLabel("Lower Threshold");
        lowerChan3 = new JTextField("1000");
        
        upperChan3Lbl.setEnabled(false);
        upperChan3.setEnabled(false);
        lowerChan3Lbl.setEnabled(false);
        lowerChan3.setEnabled(false);
         
        comp.add(chan3En);
        comp.add(upperChan3Lbl);
        comp.add(upperChan3);
        comp.add(lowerChan3Lbl);
        comp.add(lowerChan3);
        channelsPane.add(comp);
        
        
        JButton confButton = new JButton("Configure Channels");
        confButton.setActionCommand("confButton");
        confButton.addActionListener(this);
        
        JPanel confButtonPanel = new JPanel();
        confButtonPanel.setLayout(new FlowLayout());
        confButtonPanel.setPreferredSize(new Dimension(300, 40));
        confButtonPanel.setMaximumSize(new Dimension(300, 40));
        confButtonPanel.add(confButton);
        
        
        JPanel chanTotalPanel = new JPanel();
        chanTotalPanel.setLayout(new BoxLayout(chanTotalPanel,  BoxLayout.Y_AXIS));
        chanTotalPanel.setPreferredSize(new Dimension(300, 320));
        chanTotalPanel.setMaximumSize(new Dimension(300, 320));
        chanTotalPanel.setBorder(BorderFactory.createTitledBorder("Channel Settings"));
        chanTotalPanel.add(channelsPane);
        chanTotalPanel.add(confButtonPanel);
        
        
        // ---------------------------------------------------------------------
        //                  LED
        // ---------------------------------------------------------------------
        JPanel bottomPanel = new JPanel(new GridLayout(1, 2));
        bottomPanel.setPreferredSize(new Dimension(300, 120));
        bottomPanel.setMaximumSize(new Dimension(300, 120));
        
        titled = BorderFactory.createTitledBorder("LED timings (us)");
        comp = new JPanel(new GridLayout(4, 2, 2, 2), false);
        comp.setPreferredSize(new Dimension(100, 120));
        comp.setBorder(titled);
        
        ledDelayLbl = new  JLabel("Delay: ");
        ledDelay = new JTextField("0");
        
        ledONLbl = new  JLabel("ON: ");
        ledON = new JTextField("100");
        
        ledLockLbl = new  JLabel("Lock:");
        ledLock = new JTextField("500");
        
        JButton ledConf = new JButton("Set");
        ledConf.setActionCommand("setLed");
        ledConf.addActionListener(this);
        
        JButton testLed = new JButton("Test");
        testLed.setActionCommand("testLed");
        testLed.addActionListener(this);
        
        comp.add(ledDelayLbl);
        comp.add(ledDelay);
        comp.add(ledONLbl);
        comp.add(ledON);
        comp.add(ledLockLbl);
        comp.add(ledLock);
        comp.add(ledConf);
        comp.add(testLed);    
        bottomPanel.add(comp);
        
        titled = BorderFactory.createTitledBorder("NOTES");
        comp = new JPanel();
        comp.setPreferredSize(new Dimension(100, 120));
        JTextArea notes = new JTextArea();
        notes.setEditable(false);
        notes.setLineWrap(true);
        notes.setWrapStyleWord(true);
        notes.setText("All threshold values are multilpied by 1000");
        comp.add(notes);
        comp.setBorder(titled);
        bottomPanel.add(comp);
        
        
        controlPanel.add(topPanel);
        controlPanel.add(chanTotalPanel);
        controlPanel.add(bottomPanel);
        controlPanel.setBorder(BorderFactory.createEmptyBorder(5, 5, 5, 5));

    }
    
    final void createChartPanel(JComponent chartPane){
        XYSeriesCollectionNew datasetChart1 = new XYSeriesCollectionNew();
        seriesP.setMaximumItemCount(5500);
        datasetChart1.addSeries(seriesP);

        XYSeriesCollectionNew datasetChart2 = new XYSeriesCollectionNew();
        seriesQ.setMaximumItemCount(5500);
        datasetChart2.addSeries(seriesQ);

        // Corrected
        chart1 = createChart(datasetChart1, -1, 30000.0,
                "Filtered values", true);

        chart2 = createChart(datasetChart2, 0, 2.0,
                "Input values", false);

        final ChartPanel chartPanel1 = new ChartPanel(chart1);
        final ChartPanel chartPanel2 = new ChartPanel(chart2);
        
        
        chartPane.add(chartPanel1);
        chartPane.add(chartPanel2);
        
        
    }

}
