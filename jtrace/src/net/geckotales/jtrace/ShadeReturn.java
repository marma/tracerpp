package net.geckotales.jtrace;

public class ShadeReturn {
    Color color = new Color(0,0,0), transparency = new Color(0,0,0), filter = new Color(0,0,0);

    public ShadeReturn() {
    }
    
    public void setColor(Color c) {
        color.set(c);
    }
    
    public Color getColor() {
        return color;
    }
    
    public void setTransparency(Color t) {
        transparency.set(t);
    }
    
    public Color getTransparency() {
        return transparency;
    }
    
    public void setFilter(Color f) {
        this.filter = f;
    }
    
    public Color getFilter() {
        return filter;
    }
}
