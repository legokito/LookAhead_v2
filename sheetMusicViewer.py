import json
import webview


class SheetMusicViewer:
    def __init__(self):
        self.current_measure = None
        self.pages = []

        self.top_segment = None
        self.bottom_segment = None

        self.window = None

    def load_score_data(self, json_path):
        with open(json_path, "r", encoding="utf-8") as f:
            data = json.load(f)

        self.pages = data["pages"]

    def read_measure_no(self, path="measureNo.txt"):
        try:
            with open(path, "r", encoding="utf-8") as f:
                return int(f.read().strip())
        except (FileNotFoundError, ValueError):
            return None

    def display_svg(self, segment, svg_path):
        if segment == "top":
            self.top_segment = svg_path
        elif segment == "bottom":
            self.bottom_segment = svg_path
        else:
            raise ValueError(f"Unknown segment: {segment}")

    def display_pages(self, top_page, bottom_page):
        if top_page is not None:
            self.display_svg("top", top_page["svg"])

        if bottom_page is not None:
            self.display_svg("bottom", bottom_page["svg"])

    def update_webview(self):
        if self.window is None:
            return

        self.window.evaluate_js(
            f"""
            setPages(
                {json.dumps(self.top_segment)},
                {json.dumps(self.bottom_segment)}
            );
            """
        )


def main():
    viewer = SheetMusicViewer()

    viewer.load_score_data("svgs/score.json")

    viewer.display_pages(
        viewer.pages[0],
        viewer.pages[1]
    )

    viewer.window = webview.create_window(
        "Sheet Music Viewer",
        "sheetMusicViewer.html",
        width=1200,
        height=800,
        resizable=True
    )

    webview.start(viewer.update_webview)


if __name__ == "__main__":
    main()