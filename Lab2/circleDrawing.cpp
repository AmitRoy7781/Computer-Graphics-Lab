void draw8Way(int x,int y)
{
    drawPixelZone(x,y,0);
    drawPixelZone(x,y,1);
    drawPixelZone(x,y,2);
    drawPixelZone(x,y,3);
    drawPixelZone(x,y,4);
    drawPixelZone(x,y,5);
    drawPixelZone(x,y,6);
    drawPixelZone(x,y,7);    
}

void circleDrawing(int r)
{
    int d = 5 - 4*r;
    int x = 0,y=r;
    draw8Way(x,y);

    while(x<y)
    {
        if(d<0){// dE
            d+= (8*x + 12);
            x++;
        }
        else{//dSE
            d+=(8*x-8*y+20);
            x++;
            y--;
        }
        draw8Way(x,y);
    }

}
