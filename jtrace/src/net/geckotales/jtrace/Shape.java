package net.geckotales.jtrace;

import java.util.*;
import javax.vecmath.*;

public interface Shape {
    public boolean inside(Point3d p, double t);
    public Set<Intersection> intersect(Ray r);
}