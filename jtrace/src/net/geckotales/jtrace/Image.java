package net.geckotales.jtrace;

import java.io.*;
import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;

public class Image {
    int width, height;
    BufferedImage bImage = null, vImage = null;
    Imager imager = new Imager();
    
    public Image(int width, int height) {
        bImage = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);        
        vImage = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);        
        this.width = width;
        this.height = height;
    }
    
    public void setPixel(int x, int y, Color c) {
        bImage.setRGB(x, y, imager.getARGB(c));
    }
    
    public void setPixel(int x, int y, Color c[]) {
        int rsum = 0, gsum = 0, bsum = 0;
        int argb[] = new int[c.length];
        
        for (int i=0;i<c.length;i++) {
            argb[i] = imager.getARGB(c[i]);
            rsum += (argb[i] & 0x00FF0000) >> 16;
            gsum += (argb[i] & 0x0000FF00) >> 8;
            bsum += (argb[i] & 0x000000FF);
        }
        
        rsum /= c.length;
        gsum /= c.length;
        bsum /= c.length;

        vImage.setRGB(x,y,0xFF000000);
        int vr = 0, vg = 0, vb = 0;
        for (int i=0;i<c.length;i++) {
            vr += Math.abs(((argb[i] & 0x00FF0000) >> 16) - rsum);
            vg += Math.abs(((argb[i] & 0x0000FF00) >> 8) - gsum);
            vb += Math.abs((argb[i] & 0x000000FF) - bsum);
        }
        
        vr /= c.length;
        vg /= c.length;
        vb /= c.length;                
        
        int result = 0xFF000000 + (rsum << 16) + (gsum << 8) + bsum;
        int variance = 0xFF000000 + (vr <<16) + (vg << 8) + vb;
        
        bImage.setRGB(x, y, result);
        vImage.setRGB(x, y, variance);
    }
        
    public BufferedImage getBufferedImage() {
        return bImage;
    }
    
    public BufferedImage getVarianceImage() {
        BufferedImage image = new BufferedImage(width, height, BufferedImage.TYPE_BYTE_GRAY);
        
        return image;
    }
    
    public void save(File file, String type) throws IOException {
        ImageIO.write(bImage, type, file);
    }
    
    public void saveVariance(File file, String type) throws IOException {
        ImageIO.write(vImage, type, file);
    }
}
