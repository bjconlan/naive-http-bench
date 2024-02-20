package main

import (
    "time"
    "net/http"
    "encoding/json"
    "math/rand"
    "log"
)

type dayForecast struct {
    Date time.Time `json:"date"`
    TemperatureC float64 `json:"temperatureC"`
    Summary string `json:"summary"`
    TemperatureF float64 `json:"temperatureF"`
}

var summaries = []string{
    "Freezing",
    "Bracing",
    "Chilly",
    "Cool",
    "Mild",
    "Warm",
    "Balmy",
    "Hot",
    "Sweltering",
    "Scorching",
}

func weatherforecast(w http.ResponseWriter, req *http.Request) {
    var forecasts [5]dayForecast;
    for i := 0; i < len(forecasts); i++ {
        temperatureC := float64(rand.Intn(75) - 20)
        forecasts[i] = dayForecast{
            Date: time.Now(),
            TemperatureC: temperatureC,
            Summary: summaries[rand.Intn(len(summaries))],
            TemperatureF: (temperatureC / float64(0.5556)) + 32,
        }
    }

    json, err := json.Marshal(forecasts);
    if err != nil {
        w.WriteHeader(500);
        log.Print("error:", err)
    } else {
        w.Header().Set("Content-Type", "application/json")
        w.Write([]byte(json))
    }
}

func main() {
    http.HandleFunc("/weatherforecast", weatherforecast)
    err := http.ListenAndServeTLS(":7027", "../server.crt", "../server.key", nil)
    if err != nil {
        log.Fatal("ListenAndServe: ", err)
    }
}