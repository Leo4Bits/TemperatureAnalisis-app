import plotly.express as px
import pandas as pd
def drawPlotly(dataFrame,status, measureError, measureHicTimes):
    fig = px.bar(
        dataFrame, 
        x="Name value",
        y="Value", 
        color="Name value", 
        text="Value", 
        title=f"Giám sát Môi trường (Trạng thái: {status})", 
        color_discrete_map={"Nhiệt độ (°C)": "green", "Độ ẩm (%)": "blue", "Chỉ số nóng bức":"red"}, 
        subtitle= f"Sai số của các giá trị đo được lần thứ {measureHicTimes}: {measureError}"
    )

    fig.update_layout(
        yaxis_range=[0, 100],# Trục Y từ 0 đến 100
        paper_bgcolor='rgba(0,0,0,0)', #Nền trong suốt
        font=dict(size=18)
    )
    return fig