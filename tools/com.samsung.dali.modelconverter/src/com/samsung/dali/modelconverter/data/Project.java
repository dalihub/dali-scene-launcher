package com.samsung.dali.modelconverter.data;

public class Project {

  public static final String TIZEN_MANIFEST = "tizen-manifest.xml";

  public Project(String path, String name, String id) {
    assert path != null;
    assert !path.isEmpty();

    mPath = path;
    mName = name;
    mId = id;
  }

  public String getPath() {
    return mPath;
  }

  public String getName() {
    return mName;
  }

  public String getId() {
    return mId;
  }

  private String mPath;
  private String mName;
  private String mId;
}
