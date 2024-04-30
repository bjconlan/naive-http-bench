package com.example.restservice;

import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@SpringBootApplication
public class RestServiceApplication {

	public static void main(String[] args) {
		SpringApplication.run(RestServiceApplication.class, args);
	}

	public record WeatherForecast(LocalDateTime dateTime, int temperatureC, WeatherForecast.Type summary) {
		enum Type {
			FREEZING("Freezing"),
			BRACING("Bracing"),
			CHILLY("Chilly"),
			COOL("Cool"),
			MILD("Mild"),
			WARM("Warm"),
			BALMY("Balmy"),
			HOT("Hot"),
			SWELTERING("Sweltering"),
			SCORCHING("Scorching");

			private final String description;

			private Type(String description) {
				this.description = description;
			}

			public String toString() {
				return description;
			}
		};

		public float getTemperatureF() {
			return 32.0f + this.temperatureC / 0.5556f;
		}
	}

	@RestController
	public class WeatherForecastController {

		@GetMapping("/weatherforecast")
		public List<WeatherForecast> getWeatherForcast() {
			final var weatherForecasts = new ArrayList<WeatherForecast>(5);
			for (int i = 0; i < 5; i++) {
				weatherForecasts.add(
					new WeatherForecast(
						LocalDateTime.now().plusDays(i),
						new Random().nextInt(-20,55),
						WeatherForecast.Type.values()[new Random().nextInt(WeatherForecast.Type.values().length)]
					)
				);
			}
			return weatherForecasts;
		}
	}
}
