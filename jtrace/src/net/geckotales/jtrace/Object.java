package net.geckotales.jtrace;

import java.util.*;
import javax.vecmath.*;

public interface Object {
    public boolean inside(Point3d p, double t);
    public Set<Intersection> intersect(Ray r);
    public SurfaceShader getSurfaceShader();
    public VolumeShader getVolumeShader();
    public Vector<SurfaceModifier> getSurfaceModifiers();
    public void setSurfaceShader(SurfaceShader shader);
    public void setVolumeShader(VolumeShader shader);
    public void addSurfaceModifier(SurfaceModifier modifier);
}