import net
import http
import encoding.json

SUMMARIES ::= [
  "Freezing",
  "Bracing",
  "Chilly",
  "Cool",
  "Mild",
  "Warm",
  "Balmy",
  "Hot",
  "Sweltering",
  "Scorching"
]

main:
  network := net.open
  tcp_socket := network.tcp_listen 7027
  print "Server on http://localhost:$tcp_socket.local_address.port/"

  server := http.Server
  server.listen tcp_socket:: | request/http.Request writer/http.ResponseWriter |
    if request.path == "/weatherforecast":
      now :=  Time.now
      data := (List 5).map:
        temperatureC := random -25 55
        {
          "date": now.utc.stringify, // no zone details on local
          "temperatureC": temperatureC,
          "summary": SUMMARIES[random SUMMARIES.size],
          "temperatureF": (temperatureC / 0.5556) + 32
        }
      writer.headers.set "Content-Type" "application/json"
      writer.write
        json.encode data

    writer.close