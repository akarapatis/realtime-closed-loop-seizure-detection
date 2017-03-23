/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author thanasis
 */
import org.jfree.data.xy.XYDataItem;
import org.jfree.data.xy.XYSeries;


public class XYSeriesNew  extends XYSeries {
    
    
    
    public XYSeriesNew(Comparable key) {
        super(key);
    }
   
    public XYSeriesNew(Comparable key, boolean autoSort) {
        super(key, autoSort, true);
    }
    
    public XYSeriesNew(Comparable key, boolean autoSort,
            boolean allowDuplicateXValues) {
        super(key, autoSort, allowDuplicateXValues);
    }
    
    
    XYDataItem getRawDataItem(int index) {
        try{
            return (XYDataItem) this.data.get(index);
        }
        catch(IndexOutOfBoundsException | NullPointerException e ){
            return (XYDataItem) this.data.get(index-1);
            
        }
        
    }
    
    @Override
    public Number getX(int index) {
         try{
            return getRawDataItem(index).getX();
        }
        catch(IndexOutOfBoundsException | NullPointerException e ){
            return getRawDataItem(index-1).getX();
            
        }
        
    }
    
    @Override
    public Number getY(int index) {
        try{
            return getRawDataItem(index).getY();
        }
        catch(IndexOutOfBoundsException | NullPointerException e ){
            return getRawDataItem(index-1).getY();
            
        }
        
        
    }
    
    
    
}
