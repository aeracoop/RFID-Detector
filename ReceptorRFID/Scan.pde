class Scan {
  int sStart; //second();  // Values from 0 - 59
  int mStart; // minute();  // Values from 0 - 59
  int hStart; // hour();    // Values from 0 - 23

  int sEnd; //second();  // Values from 0 - 59
  int mEnd; // minute();  // Values from 0 - 59
  int hEnd; // hour();    // Values from 0 - 23

  StringList ids;  

  Scan() {
    sStart = second();  // Values from 0 - 59
    mStart = minute();  // Values from 0 - 59
    hStart = hour();    // Values from 0 - 23
    ids = new StringList();
  }

  void add(String id) {
    ids.append(id);
  }

  int items() {
    return ids.size();
  }

  String last() {
    return ids.get(ids.size()-1);
  }

  public String fileName() {
    String fileName;
    int d = day();    // Values from 1 - 31
    int m = month();  // Values from 1 - 12
    int y = year();   // 2003, 2004, 2005, etc.
    fileName = "RFID-"+d+"-"+m+"-"+y+"-"+hStart+":"+mStart+":"+sStart+".txt";
    return fileName;
  }

  public void save() {
    saveStrings(fileName(), ids.array());
  }
}
