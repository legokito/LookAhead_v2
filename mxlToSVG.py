import verovio
import json
import sys
import xml.etree.ElementTree as ET
import os

# template file in cpp calls this file with xml filepath as arg
if len(sys.argv) != 2:
    raise ValueError("Expected MusicXML path")
music_xml_path = sys.argv[1]

tk = verovio.toolkit()

# svg sizing and stuff
tk.setOptions({
    "systemMaxPerPage": 2,
    "footer": "none",

    "scale": 40,

    "pageWidth": 2400,
    "pageHeight": 1300,

    "pageMarginTop": 20,
    "pageMarginBottom": 20,
    "pageMarginLeft": 20,
    "pageMarginRight": 20,

    "spacingStaff": 1,
    "adjustPageHeight": True,

    "svgViewBox": True,
    "expand": "always",
})

tk.loadFile(music_xml_path)
tk.redoLayout()

os.makedirs("svgs", exist_ok=True)

# render svgs
for page in range(1, tk.getPageCount() + 1):
    svg = tk.renderToSVG(page)
    path = f"svgs/page_{page}.svg"
    with open(path, "w", encoding="utf-8") as f:
        f.write(svg)

# collect info from svg metadata and put into json file for sheetMusicViewer 
pages = []
page_mei = tk.getMEI({
    "scoreBased": False
})

root = ET.fromstring(page_mei)
for page_elem in root.findall(".//{*}page"):
    systems = page_elem.findall(".//{*}system")
    measures = []

    for system in systems:
        measures.extend(system.findall(".//{*}measure"))

    pages.append({
        "svg": f"page_{len(pages) + 1}.svg",
        "startMeasure": measures[0].get("n"),
        "endMeasure": measures[-1].get("n")
    })

metadata = {
    "pages": pages
}

with open("svgs/score.json", "w", encoding="utf-8") as f:
    json.dump(metadata, f, indent=2)

