package net.geckotales.jtrace;

import javax.vecmath.*;

public class Ray {
    Point3d origin;
    Vector3d direction;
    Color c = new Color(0,0,0);
    double weight = 1.0, t = 0.0;
    
    public Ray() {
        origin = new Point3d(0,0,0);
        direction = new Vector3d(0,0,1);
    }
    
    public Ray(Point3d o, Vector3d d) {
        origin = new Point3d(o);
        direction = new Vector3d(d);
    }
    
    public Ray(Point3d p1, Point3d p2, double t) {
        origin = new Point3d(p1);
        direction = new Vector3d(p2);
        direction.sub(p1);
        direction.normalize();
        this.t = t;
    }
    
    public Ray(Point3d o, Vector3d d, double t) {
        origin = new Point3d(o);
        direction = new Vector3d(d);
        this.t = t;
    }
    
    public Point3d getOrigin() {
        return origin;
    }
    
    public Vector3d getDirection() {
        return direction;
    }
    
    public void setOrigin(Point3d o) {
        origin.set(o);
    }
    
    public void setDirection(Vector3d d) {
        direction.set(d);
    }
    
    public Color getColor() {
        return c;
    }
    
    public double getWeight() {
        return weight;
    }
    
    public void setColor(Color c) {
        this.c.set(c);
    }
    
    public double getTime() {
        return t;
    }
    
    public void setTime(double t) {
        this.t = t;
    }
    
    public void setWeight(double w) {
        weight = w;
    }
    
    public String toString() {
        return "ray { origin=" + origin + ", direction=" + direction + ", color=" + c + ", t=" + t + " }";
    }
}