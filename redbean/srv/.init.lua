local function iso_8601_timestamp()
	local secs, nanos = unix.clock_gettime(CLOCK_REALTIME)
	return os.date("!%Y-%m-%dT%T", secs) .. "." .. math.floor(nanos / 1000) .. "Z" -- need to add zone here
end

local SUMMARIES <const> = {
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
}

local function BuildRecords(count)
	local records = {}
	for i=1,count do
		local tempC = math.random(-20, 55);
		table.insert(records, {
			date = iso_8601_timestamp(),
			temperatureC = tempC,
			summary = SUMMARIES[math.random(#SUMMARIES)],
			temperatureF = 32 + (tempC / 0.5556)
		})
	end
	return records
end

function OnHttpRequest()
	if "/weatherforecast" == GetPath() then
		SetStatus(200)
		SetHeader('Content-Type', 'application/json; charset=utf-8')
		Write(EncodeJson(BuildRecords(5)))
	else
		Route()
	end
end