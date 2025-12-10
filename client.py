import socket
import matplotlib.pyplot as plt

ESP_IP = "192.168.4.1"
ESP_PORT = 5000
APP_PASSWORD = "login_bursttest"
plt.style.use({
    'axes.facecolor': '#1a1a1a',
    'axes.edgecolor': 'gray',
    'axes.labelcolor': 'white',
    'text.color': 'white',
    'xtick.color': 'tab:blue',
    'ytick.color': 'tab:blue',
    'grid.color': 'gray',
    'figure.facecolor': '#1a1a1a',
    'figure.edgecolor': '#1a1a1a',
    'savefig.facecolor': '#1a1a1a',
    'savefig.edgecolor': '#1a1a1a',
})


def configure_socket(ESP_IP, ESP_PORT, APP_PASSWORD):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((ESP_IP, ESP_PORT))

        s.sendall((APP_PASSWORD + "\n").encode())
        resp = s.recv(1024).decode().strip()

        if resp != "AUTH OK":
            print("Auth failed:", resp)
            return

        print("Authenticated!")
        f = s.makefile("r")


        plt.ion()
        fig, ax = plt.subplots()
        ax.set_title("Tank Pressure")
        ax.set_xlabel('time(s)')
        ax.grid(True)
        ax.set_ylabel('pressure(bar)')
        xdata, ydata = [], []
        line, = ax.plot([], [], "-o")
        ax.set_ylim(0, 22)

        t = 0.0

        while True:
            raw = f.readline()
            if not raw:
                break

            raw = raw.strip()
            try:
                value = float(raw)
            except ValueError:
                print("Skipped malformed:", raw)
                continue

            t += 0.05
            xdata.append(t)
            ydata.append(value)
            #print(value)
            update_plot(fig, ax, line, xdata, ydata)


def update_plot(fig, ax, line, xdata, ydata):
    line.set_xdata(xdata)
    line.set_ydata(ydata)
    ax.set_title("Tank Pressure: " + str(ydata[-1]) + "bar")
    ax.relim()
    ax.autoscale_view()

    fig.canvas.draw()
    fig.canvas.flush_events()


if __name__ == "__main__":
    configure_socket(ESP_IP, ESP_PORT, APP_PASSWORD)
