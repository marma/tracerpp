package net.geckotales.jtrace;

import javax.vecmath.*;

public class VecTest {
    public static void main(String args[]) {
        Matrix4d A = new Matrix4d();
        Vector3d n = new Vector3d(1,1,0);
        n.normalize();
        Point3d p = new Point3d(n);
        
        A.setIdentity();
        
        A.transform(n);
        A.transform(p);
        
        System.out.println(p);
        System.out.println(255 >> 2);
    }
}