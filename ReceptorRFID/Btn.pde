class Btn
{
  private int x, y, width, height;
  private String label;
  private int fontSize;

  public Btn(int x, int y, String label)
  {
    this.x = x;
    this.y = y;
    this.width = (w/2)+40; 
    this.height = (h/10)-10;
    this.fontSize = 10;
    this.label = label;
  }

  public Btn(int x, int y, int width, int height, String label, int fontSize)
  {
    this.x = x;
    this.y = y;
    this.width = width;
    this.height = height;
    this.fontSize = fontSize;
    this.label = label;
  }

  public void draw()
  {
    fill(5, 150, 250, 200);
    stroke(0);
    strokeWeight(4);
    strokeCap(ROUND);
    rect(x, y, width, height);
    noStroke();
    fill(0);
    text(label, x + (width/2), y + (height - fontSize) / 2 );
  }

  public void setLabel(String l)
  {
    this.label = l;
  }

  public boolean isPressed()
  {    
    if (!mousePressed)
      return false;  
    if (mouseX >= x && mouseX <= x + width)
    {
      if (mouseY >= y && mouseY <= y + (height))      
        return true;
    }
    return false;
  }
}
