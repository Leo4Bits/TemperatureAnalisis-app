from flask import Flask, render_template
import requests
import plotly.express as px
import pandas as pd
from DrawChart import*

app = Flask(__name__)

ESP32_URL_WEBSERVER = "http://192.168.1.50" 

@app.route('/', methods=['GET'])
def index():
    try:
        response = requests.get(ESP32_URL_WEBSERVER, timeout=2)
        data_esp = response.json()
        
        temp = data_esp.get("temp", 0)
        hum = data_esp.get("hum", 0)
        hic = data_esp.get("hic",0)
        measureError = data_esp.get("measureError",0)
        measureHicTimes = data_esp.get("measureHifTimes",0)
        status = "Connected"
    except Exception as e:
        print("Lỗi:", e)
        temp = 0
        hum = 0
        hic = 0
        measureError = 0
        measureHicTimes = 0
        status = "Disconnected to ESP32"

    # Pandas -> create table data
    # 3 col for temp - hum - hif
    df = pd.DataFrame({
        "Name value": ["Nhiệt độ (°C)", "Độ ẩm (%)", "Chỉ số nóng bức"],
        "Value": [temp, hum, hic]
    })

    # Convert Py chart 2 HTML Chart
    # full_html=False nghĩa là chỉ lấy cái khung biểu đồ thôi, nhét vào web mình
    graph_html = drawPlotly(df,status,measureError,measureHicTimes).to_html(full_html=False)
    return render_template('index.html', graph_html=graph_html)
    

    # sent to template
    

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)