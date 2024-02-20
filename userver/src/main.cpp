#include <chrono>

#include <userver/components/minimal_server_component_list.hpp>
#include <userver/formats/json/string_builder.hpp>
#include <userver/http/content_type.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/secdist/component.hpp>
#include <userver/storages/secdist/provider_component.hpp>
#include <userver/utils/daemon_run.hpp>
#include <userver/utils/datetime.hpp>
#include <userver/utils/rand.hpp>

using namespace userver;

namespace routes {

class WeatherForecast final : public server::handlers::HttpHandlerBase {
public:
  static constexpr std::string_view kName = "handler-weatherforecast";
  static constexpr std::array<std::string_view, 10> kSummary = {
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
  };

  using HttpHandlerBase::HttpHandlerBase;

  std::string HandleRequestThrow(
      const server::http::HttpRequest& req,
      server::request::RequestContext&) const override {
    using namespace userver::formats;

    req.GetHttpResponse().SetContentType(http::ContentType("application/json"));

    json::StringBuilder sb;
    {
      json::StringBuilder::ArrayGuard guard{sb};
      const auto dateTime = utils::datetime::Now();
      for (int i = 0; i < 5; i++) {
        const auto temperatureC = utils::RandRange(-20, 56);
        json::StringBuilder::ObjectGuard oguard{sb};
        sb.Key("date");
        json::WriteToStream(utils::datetime::Timestring(dateTime + std::chrono::hours(24 * i)), sb);
        sb.Key("temperatureC");
        json::WriteToStream(temperatureC, sb);
        sb.Key("summary");
        json::WriteToStream(kSummary.at(utils::RandRange(kSummary.size())), sb);
        sb.Key("temperatureF");
        json::WriteToStream((temperatureC/0.5556) + 32, sb);
      }
    }

    return sb.GetString();
  }
};

} // routes namespace

int main(int argc, char* argv[]) {
  return utils::DaemonMain(argc, argv, components::MinimalServerComponentList()
                                           .Append<routes::WeatherForecast>()
                                           .Append<components::DefaultSecdistProvider>()
                                           .Append<components::Secdist>()
                           );
}
