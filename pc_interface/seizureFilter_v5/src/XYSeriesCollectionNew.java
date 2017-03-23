
import java.util.List;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author thanasis
 */
class XYSeriesCollectionNew extends XYSeriesCollection {
     private List data;
    
    @Override
    public void addSeries(XYSeries series) {
        super.addSeries(series);
        
        data = super.getSeries();
        
    }

    
    @Override
    public Number getX(int series, int item) {
        XYSeriesNew s = (XYSeriesNew) this.data.get(series);
        return s.getX(item);
    }
    
    
}
