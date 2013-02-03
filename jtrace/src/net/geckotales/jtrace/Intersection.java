package net.geckotales.jtrace;

import javax.vecmath.*;

public class Intersection implements Comparable {
    protected Object object = null;
    protected Point3d position = new Point3d();
    protected Point3d globalPosition = new Point3d();
    protected Vector3d normal = new Vector3d();
    protected Vector3d globalNormal = new Vector3d();
    protected double s,t,u,v,sortValue=0.0;
    protected int type;
    public static int IN      = 1;
    public static int OUT     = 2;
    public static int UNKNOWN = 3;
    
    public Intersection() {
    }
    
    public void setPosition(Tuple3d position) {
        this.position.set(position);
    }
    
    public Point3d getPosition() {
        return position;
    }
    
    public void setNormal(Tuple3d normal) {
        this.normal.set(normal);
    }
    
    public Vector3d getNormal() {
        return normal;
    }

    public void setGlobalPosition(Tuple3d position) {
        this.globalPosition.set(position);
    }
    
    public void setSortValue(double s) {
        sortValue = s;
    }
    
    public Point3d getGlobalPosition() {
        return globalPosition;
    }
    
    public void setGlobalNormal(Tuple3d normal) {
        this.globalNormal.set(normal);
    }
    
    public Vector3d getGlobalNormal() {
        return globalNormal;
    }
    
    public int getType() {
        return type;
    }
    
    public void setType(int type) {
        this.type = type;
    }
    
    public void setObject(Object o) {
        object = o;
    }
    
    public Object getObject() {
        return object;
    }
    
    public double getSortValue() {
        return sortValue;
    }
    
    public int compareTo(java.lang.Object o) {
        return Double.compare(sortValue, ((Intersection)o).getSortValue());
    }
}