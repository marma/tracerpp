package net.geckotales.jtrace;

public class Color {
    double r,g,b;
    public static Color WHITE = new Color(1,1,1);
    public static Color BLACK = new Color(0,0,0);
    public static Color RED =   new Color(1,0,0);
    public static Color GREEN = new Color(0,1,0);
    public static Color BLUE =  new Color(0,0,1);
    
    public Color() {
        r = g = b;
    }
    
    public Color(Color c) {
        this.r = c.r;
        this.g = c.g;
        this.b = c.b;
    }
    
    public Color(double r, double g, double b) {
        this.r = r;
        this.g = g;
        this.b = b;
    }
    
    public void set(double r, double g, double b) {
        this.r = r;
        this.g = g;
        this.b = b;
    }
    
    public void set(Color c) {
        r = c.r;
        g = c.g;
        b = c.b;
    }
    
    public Color add(Color c) {
        r += c.r;
        g += c.g;
        b += c.b;
        
        return this;
    }
    
    public Color sub(Color c) {
        r -= c.r;
        g -= c.g;
        b -= c.b;
        
        return this;
    }
    
    public Color mul(Color c) {
        r *= c.r;
        g *= c.g;
        b *= c.b;
        
        return this;
    }
    
    public Color mul(double d) {
        r *= d;
        g *= d;
        b *= d;
        
        return this;
    }
    
    public static Color add(Color c1, Color c2) {
        return new Color(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b);
    }
    
    public static Color sub(Color c1, Color c2) {
        return new Color(c1.r - c2.r, c1.g - c2.g, c1.b - c2.b);
    }
    
    public static Color mul(double d, Color c) {
        return new Color(d*c.r, d*c.g, d*c.b);
    }
    
    public static Color mul(Color c1, Color c2) {
        return new Color(c1.r * c2.r, c1.g * c2.g, c1.b * c2.b);
    }
    
    public double value() {
        return 0.299*r+0.587*g+0.114*b;
    }
    
    public Color exp() {
        return new Color(Math.exp(r), Math.exp(g), Math.exp(b));
    }
    
    public Color exp(double d) {
        return new Color(Math.exp(d*r), Math.exp(d*g), Math.exp(d*b));
    }
    
    public Color pow(double d) {
        return new Color(Math.pow(r,d), Math.pow(g,d), Math.pow(b,d));
    }
    
    public Color abs() {
        return new Color(Math.abs(r), Math.abs(g), Math.abs(b));
    }
    
    public Color clamp(double min, double max) {
        Color c = new Color(this);
        c.r = (r<min)?min:r; c.r = (r>max)?max:r;
        c.g = (g<min)?min:g; c.g = (g>max)?max:g;
        c.b = (b<min)?min:b; c.b = (b>max)?max:b;
        
        return c;
    }
    
    public Color map(double out_min, double out_max) {
        double in_min = 0.0, in_max = 1.0;
        double s = (out_max - out_min)/(in_max - in_min);
        
        return new Color((r-in_min)*s + out_min, (g-in_min)*s + out_min, (b - in_min)*s + out_min);
    }    
    
    public Color map(double out_min, double out_max, double in_min, double in_max) {
        double s = (out_max - out_min)/(in_max - in_min);
        
        return new Color((r-in_min)*s + out_min, (g-in_min)*s + out_min, (b - in_min)*s + out_min);
    }    
}
