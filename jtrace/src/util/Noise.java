package net.geckotales.jtrace.util;

import javax.vecmath.*;

public class Noise {
    public static double noise(double x, double y, double z) {
        return ImprovedNoise.noise(x, y, z);
    }
    
    public static double fBm(double x, double y, double z, double H, double lacunarity, double octaves) {
        double nx = x, ny = y, nz = z, value = 0, l = 1;
        
        for (int i=0;i<octaves;i++) {
            value += noise(nx,ny,nz)*Math.pow(l, -H);
            l *= lacunarity;
            nx *= lacunarity;
            ny *= lacunarity;
            nz *= lacunarity;            
        }
        
        return value;
    }
    
    public static double VLnoise(double x, double y, double z, double d) {
        double ox = noise(x + 0.5, y, z), oy = noise(x, y + 0.5, z), oz = noise(x, y, z + 0.5);
        
        return noise(x + d*ox, y + d*oy, z + d*oz);
    }
        
    public static double VLfBm(double x, double y, double z, double d, double H, double lacunarity, double octaves) {
        double nx = x, ny = y, nz = z, value = 0, l = 1;
        
        for (int i=0;i<octaves;i++) {
            value += VLnoise(nx,ny,nz, d)*Math.pow(l, -H);
            l *= lacunarity;
            nx *= lacunarity;
            ny *= lacunarity;
            nz *= lacunarity;            
        }
        
        return value;
    }
    
    public static double noise(Tuple3d x) {
        return ImprovedNoise.noise(x.x, x.y, x.z);
    }
    
    public static double fBm(Tuple3d x, double H, double lacunarity, double octaves) {
        return fBm(x.x, x.y, x.z, H, lacunarity, octaves);
    }
    
    public static double VLnoise(Tuple3d x, double d) {
        return VLnoise(x.x, x.y, x.z, d);
    }
    
    public static double VLfBm(Tuple3d x, double d, double H, double lacunarity, double octaves) {
        return VLfBm(x.x, x.y, x.z, d, H, lacunarity, octaves);
    }
    
    public static Vector3d dnoise(double x, double y, double z) {
        double dx = 0.00001, n = noise(x, y, z);
        
        return new Vector3d((noise(x + dx, y, z) - n)/dx, (noise(x, y + dx, z) - n)/dx, (noise(x, y, z + dx) - n)/dx);   
    }
    
    public static Vector3d dfBm(double x, double y, double z, double H, double lacunarity, double octaves) {
        double dx = 0.00001, n = fBm(x, y, z, H, lacunarity, octaves);
        
        return new Vector3d((fBm(x + dx, y, z, H, lacunarity, octaves) - n)/dx, (fBm(x, y + dx, z, H, lacunarity, octaves) - n)/dx, (fBm(x, y, z + dx, H, lacunarity, octaves) - n)/dx);   
    }
    
    public static Vector3d dVLnoise(double x, double y, double z, double d) {
        double dx = 0.00001, n = VLnoise(x, y, z, d);
        
        return new Vector3d((VLnoise(x + dx, y, z, d) - n)/dx, (VLnoise(x, y + dx, z, d) - n)/dx, (VLnoise(x, y, z + dx, d) - n)/dx);   
    }
    
    public static Vector3d dVLfBm(double x, double y, double z, double d, double H, double lacunarity, double octaves) {
        double dx = 0.00001, n = VLfBm(x, y, z, d, H, lacunarity, octaves);
        
        return new Vector3d((VLfBm(x + dx, y, z, d, H, lacunarity, octaves) - n)/dx, (VLfBm(x, y + dx, z, d, H, lacunarity, octaves) - n)/dx, (VLfBm(x, y, z + dx, d, H, lacunarity, octaves) - n)/dx);   
    }
    
    public static Vector3d dnoise(Tuple3d x) {
        return dnoise(x.x, x.y, x.z);
    }    
    
    public static Vector3d dfBm(Tuple3d x, double H, double lacunarity, double octaves) {
        return dfBm(x.x, x.y, x.z, H, lacunarity, octaves);
    }    
    
    public static Vector3d dVLnoise(Tuple3d x, double d) {
        return dVLnoise(x.x, x.y, x.z, d);
    }
    
    public static Vector3d dVLfBm(Tuple3d x, double d, double H, double lacunarity, double octaves) {
        return dVLfBm(x.x, x.y, x.z, d, H, lacunarity, octaves);
    }    
}
