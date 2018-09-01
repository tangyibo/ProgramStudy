package com.learn.module;

import java.io.Serializable;

public class CoordinatePoint implements Serializable{
        public float x;
        public float y;
        public float z;

        public CoordinatePoint(float x,float y,float z){
                this.x=x;
                this.y=y;
                this.z=z;
        }
}
