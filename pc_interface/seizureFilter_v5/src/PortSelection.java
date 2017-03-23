
import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.Action;
import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SpringLayout;
import org.jfree.ui.ApplicationFrame;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/**
 *
 * @author thanasis
 */
public class PortSelection extends JDialog implements ActionListener {

    JComboBox portsList;
    PlotGUI parent;
    
    public PortSelection(PlotGUI parent, String[] ports) {

        super(parent);
        this.parent = parent;
        
        portCheck(ports);
        
        
        this.setTitle("Serial port");
        this.setSize(new Dimension(300, 100));
        this.setLocationRelativeTo(null);
        this.setVisible(true);
        this.setDefaultCloseOperation(JDialog.DO_NOTHING_ON_CLOSE);
        
    }
    
    private void portCheck(String [] ports){
         if (ports.length == 0) {
            checkAgain();
        } else {
            createDialog(ports);
        }
    }
    
    void checkAgain(){
        Container content = this.getContentPane();
        content.removeAll();
        content.setLayout(new GridLayout(2, 1));

        JButton SelectBt = new JButton("Check again");
        SelectBt.setActionCommand("Check");
        SelectBt.addActionListener(this);

        JButton ExitBt = new JButton("Exit");
        ExitBt.setActionCommand("Exit");
        ExitBt.addActionListener(this);

        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new FlowLayout());
        buttonPanel.add(SelectBt);
        buttonPanel.add(ExitBt);

        JPanel portsListPanel = new JPanel();
        portsListPanel.setLayout(new FlowLayout(FlowLayout.CENTER));
        portsListPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));

        JLabel portsLabel = new JLabel("No serial device conected");
        portsListPanel.add(portsLabel);
        
        content.add(portsListPanel);
        content.add(buttonPanel);
        content.revalidate();
    }
    
    void createDialog(String[] ports) {
        Container content = this.getContentPane();
        content.removeAll();
        content.setLayout(new GridLayout(2, 1));

        JButton SelectBt = new JButton("Select");
        SelectBt.setActionCommand("Select");
        SelectBt.addActionListener(this);

        JButton ExitBt = new JButton("Exit");
        ExitBt.setActionCommand("Exit");
        ExitBt.addActionListener(this);

        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new FlowLayout());
        buttonPanel.add(SelectBt);
        buttonPanel.add(ExitBt);

        JPanel portsListPanel = new JPanel();
        portsListPanel.setLayout(new FlowLayout(FlowLayout.CENTER));
        portsListPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));

        JLabel portsLabel = new JLabel("Select port");
        portsListPanel.add(portsLabel);
        portsList = new JComboBox(ports);
        portsList.setSelectedIndex(0);
        Dimension d = portsList.getPreferredSize();
        portsList.setPreferredSize(new Dimension(150, d.height));

        portsListPanel.add(portsList);

        content.add(portsListPanel);
        content.add(buttonPanel);
        content.revalidate();

    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getActionCommand().equals("Select")) {
//            parent.createGUI
            parent.createGUI((String) portsList.getSelectedItem());
            this.dispose();

        } else if (e.getActionCommand().equals("Exit")) {
            System.exit(0);
        }
        else if (e.getActionCommand().equals("Check")) {
            portCheck(PlotGUI.getAvailableSerialPorts());
        }

    }

}
