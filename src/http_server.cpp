#define CPPHTTPLIB_OPENSSL_SUPPORT 0
#include "httplib.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <filesystem>
#include <random>

using namespace std;
namespace fs = std::filesystem;

string random_filename() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 999999);
    return "/tmp/graph_" + to_string(dis(gen));
}

string get_html() {
    return "<!DOCTYPE html>\n"
"<html>\n"
"<head><title>Graph Visualizer</title>\n"
"<style>\n"
"body { font-family: Arial; background: #1a1a2e; color: white; padding: 20px; }\n"
".container { max-width: 1200px; margin: 0 auto; }\n"
".flex { display: flex; gap: 20px; flex-wrap: wrap; }\n"
".panel { background: rgba(255,255,255,0.1); border-radius: 10px; padding: 20px; flex: 1; }\n"
".preview { flex: 2; background: white; border-radius: 10px; padding: 10px; min-height: 500px; }\n"
"textarea { width: 100%; height: 200px; background: #1e1e2e; color: #0f0; border: 1px solid #333; padding: 10px; font-family: monospace; }\n"
"button { background: #4a90e2; color: white; border: none; padding: 10px 20px; margin: 5px; border-radius: 5px; cursor: pointer; }\n"
".success { background: #27ae60; }\n"
"input { width: 100%; padding: 8px; margin: 5px 0; background: #1e1e2e; border: 1px solid #333; color: white; border-radius: 5px; }\n"
"h3 { margin-top: 0; }\n"
"</style>\n"
"</head>\n"
"<body>\n"
"<div class='container'>\n"
"<h1>Graph Visualizer</h1>\n"
"<div class='flex'>\n"
"<div class='panel'>\n"
"<h3>Input Graph</h3>\n"
"<textarea id='graph'>4\n1 2\n0 2 3\n0 1\n1</textarea>\n"
"<div>\n"
"<button onclick=\"loadPreset('simple')\">Simple</button>\n"
"<button onclick=\"loadPreset('directed')\">Directed</button>\n"
"<button onclick=\"loadPreset('complete')\">Complete K5</button>\n"
"</div>\n"
"<h3>Settings</h3>\n"
"<label>Width:</label><input type='number' id='width' value='800'>\n"
"<label>Height:</label><input type='number' id='height' value='600'>\n"
"<label>Radius:</label><input type='number' id='radius' value='20'>\n"
"<label><input type='checkbox' id='directed'> Directed</label>\n"
"<button class='success' onclick='generate()'>Generate</button>\n"
"<button onclick='downloadSVG()'>Download SVG</button>\n"
"</div>\n"
"<div class='preview' id='result'>\n"
"<svg width='400' height='300'><rect width='100%' height='100%' fill='#f5f5f5'/><text x='200' y='150' fill='#999' text-anchor='middle'>Click Generate</text></svg>\n"
"</div>\n"
"</div>\n"
"</div>\n"
"<script>\n"
"const presets = {\n"
"    simple: '4\\n1 2\\n0 2 3\\n0 1\\n1',\n"
"    directed: '4\\n1\\n2\\n3\\n0',\n"
"    complete: '5\\n1 2 3 4\\n0 2 3 4\\n0 1 3 4\\n0 1 2 4\\n0 1 2 3'\n"
"};\n"
"let currentSVG = '';\n"
"function loadPreset(type) {\n"
"    document.getElementById('graph').value = presets[type];\n"
"    if(type === 'directed') document.getElementById('directed').checked = true;\n"
"    generate();\n"
"}\n"
"async function generate() {\n"
"    const params = new URLSearchParams({\n"
"        graph: document.getElementById('graph').value,\n"
"        width: document.getElementById('width').value,\n"
"        height: document.getElementById('height').value,\n"
"        radius: document.getElementById('radius').value,\n"
"        directed: document.getElementById('directed').checked\n"
"    });\n"
"    const resp = await fetch('/generate', { method: 'POST', body: params });\n"
"    const svg = await resp.text();\n"
"    currentSVG = svg;\n"
"    document.getElementById('result').innerHTML = svg;\n"
"}\n"
"function downloadSVG() {\n"
"    if(currentSVG) {\n"
"        const blob = new Blob([currentSVG], {type: 'image/svg+xml'});\n"
"        const a = document.createElement('a');\n"
"        a.href = URL.createObjectURL(blob);\n"
"        a.download = 'graph.svg';\n"
"        a.click();\n"
"    }\n"
"}\n"
"loadPreset('simple');\n"
"</script>\n"
"</body>\n"
"</html>\n";
}

int main() {
    httplib::Server svr;
    
    svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
        res.set_content(get_html(), "text/html");
        cout << "GET / - HTML sent" << endl;
    });
    
    svr.Post("/generate", [](const httplib::Request& req, httplib::Response& res) {
        string graph_data = req.get_param_value("graph");
        int width = stoi(req.get_param_value("width"));
        int height = stoi(req.get_param_value("height"));
        int radius = stoi(req.get_param_value("radius"));
        bool directed = req.get_param_value("directed") == "true";
        
        string input_file = random_filename() + ".txt";
        ofstream f(input_file);
        f << graph_data;
        f.close();
        
        string output_file = random_filename() + ".svg";
        string cmd = "./graph_vis -i " + input_file + " -o " + output_file +
                     " -width " + to_string(width) + " -height " + to_string(height) +
                     " -radius " + to_string(radius);
        if (directed) cmd += " -directed";
        
        system(cmd.c_str());
        
        ifstream svg_file(output_file);
        string svg((istreambuf_iterator<char>(svg_file)), istreambuf_iterator<char>());
        
        fs::remove(input_file);
        fs::remove(output_file);
        
        res.set_content(svg, "image/svg+xml");
        cout << "POST /generate - SVG sent (" << svg.size() << " bytes)" << endl;
    });
    
    svr.Get("/favicon.ico", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("", "image/x-icon");
    });
    
    cout << "\n========================================" << endl;
    cout << "   C++ HTTP Server with Graph Visualizer" << endl;
    cout << "========================================" << endl;
    cout << "Server running at http://localhost:8080" << endl;
    cout << "Press Ctrl+C to stop" << endl;
    cout << "========================================\n" << endl;
    
    svr.listen("localhost", 8080);
    
    return 0;
}
