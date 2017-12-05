package com.samsung.dali.modelconverter.data;

/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Element;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import com.samsung.dali.modelconverter.data.document.Document;

public class Project {

  public static Project open(String path) throws ParserConfigurationException, SAXException, IOException {
    if (!path.endsWith(File.separator)) {
      path += File.separator;
    }

    File manifest = new File(path + TIZEN_MANIFEST);
    Project project = null;
    if (!manifest.exists()) {
      throw new FileNotFoundException(manifest.getAbsolutePath() + " was not found on the system.");
    }

    DocumentBuilder builder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
    org.w3c.dom.Document xmlDoc = builder.parse(manifest);

    Element elem = xmlDoc.getDocumentElement();
    elem.normalize();

    NodeList elems = elem.getElementsByTagName("watch-application");
    assert elems.getLength() == 1;

    Element watchAppElem = (Element) elems.item(0);
    String id = watchAppElem.getAttribute("appid");

    elems = watchAppElem.getElementsByTagName("label");
    assert elems.getLength() > 0;

    // TODO: there may be multiple labels, don't just take the first one.
    Element labelElem = (Element) elems.item(0);
    String name = labelElem.getTextContent();

    project = new Project(path, name, id);

    return project;
  }

  public static final String TIZEN_MANIFEST = "tizen-manifest.xml";

  public static final String MODELS_PATH = "res" + File.separator + "models" + File.separator + "scenes"
      + File.separator;
  public static final String IMAGES_PATH = "res" + File.separator + "images" + File.separator + "scenes"
      + File.separator;
  public static final String SHADERS_PATH = "res" + File.separator + "shaders" + File.separator + "scenes"
      + File.separator;

  public static final String SCENE_DLI = "scene.dli";
  public static final String SCENE_BIN = "scene.bin";

  public Project(String path, String name, String id) {
    assert path != null;
    assert !path.isEmpty();

    if (!path.endsWith(File.separator)) {
      path += File.separator;
    }

    mPath = path;
    mName = name;
    mId = id;
    mDocument = new Document();
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

  public String getModelsPath() {
    return mPath + MODELS_PATH;
  }

  public String getImagesPath() {
    return mPath + IMAGES_PATH;
  }

  public String getShadersPath() {
    return mPath + SHADERS_PATH;
  }

  public String getSceneDliPath() {
    return getModelsPath() + SCENE_DLI;
  }

  public String getSceneBinPath() {
    return getModelsPath() + SCENE_BIN;
  }

  public Document getDocument() {
    return mDocument;
  }

  public void setDocument(Document doc) {
    mDocument = doc;
  }

  public void writeDocument() throws IOException {
    try (FileWriter fw = new FileWriter(getSceneDliPath())) {
      fw.write(mDocument.toDliString());
    }
  }

  private String mPath;
  private String mName;
  private String mId;
  private Document mDocument;
}
