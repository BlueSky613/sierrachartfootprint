#include "sierrachart.h"
#include <windows.h>
#include <winhttp.h>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#pragma comment(lib, "winhttp.lib")

SCDLLName("RecentCandleFootprintLogger");

SCSFExport scsf_RecentCandleFootprintLogger(SCStudyInterfaceRef sc)
{
    if (sc.SetDefaults)
    {
        sc.GraphName = "Recent Candle Footprint Logger";
        sc.StudyDescription = "Logs footprint data (price, ask volume, bid volume) for the most recent candle.";

        sc.AutoLoop = 0; // Manual looping
        return;
    }

    // Check if there's at least one bar on the chart
    if (sc.ArraySize < 1)
        return;

    // Get the index of the most recent candle
    int recentBarIndex = sc.ArraySize - 2;
    static int lastProcessedBarIndex = -1;
    if (lastProcessedBarIndex != sc.UpdateStartIndex)
    {
        std::vector<std::string> jsonArray;
        for (float price = sc.Low[recentBarIndex]; price <= sc.High[recentBarIndex] + 0.05; price += sc.TickSize)
        {
            int askVolume = sc.VolumeAtPriceForBars->GetAskVolumeAtPrice(recentBarIndex, price / 0.05);
            int bidVolume = sc.VolumeAtPriceForBars->GetBidVolumeAtPrice(recentBarIndex, price / 0.05);

            std::string json_info = R"({"price": )" + std::to_string(price > sc.High[recentBarIndex] ? sc.High[recentBarIndex] : price) + R"(, "askVolume": )" + std::to_string(askVolume) + R"(, "bidVolume": )" + std::to_string(bidVolume) + "}";

            jsonArray.push_back(json_info);

            // // Prepare the message to log
            // SCString logMessage;
            // logMessage.Format("Recent Bar: %d, Price: %.2f, Ask Volume: %d, Bid Volume: %d", recentBarIndex, price, askVolume, bidVolume);
            // // Log the message
            // sc.AddMessageToLog(logMessage, 0);
        }
        std::ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < jsonArray.size(); ++i)
        {
            if (i != 0)
            {
                oss << ",";
            }
            oss << jsonArray[i];
        }
        oss << "]";
        std::string jsonArrayStr = oss.str();
        lastProcessedBarIndex = sc.UpdateStartIndex;
        SCDateTime barDateTime = sc.BaseDateTimeIn[recentBarIndex];
        // SCString barDateTimeStr = sc.DateTimeToString(barDateTime.ToUNIXTime(), FLAG_DT_COMPLETE_DATETIME);
        time_t unixTimestamp = barDateTime.ToUNIXTime();
        SCString jsonPayload;
        SCString unixTimestampStr;
        unixTimestampStr.Format("%lld", static_cast<long long>(unixTimestamp));
        jsonPayload.Format("{\"time\": \"%s\", \"open\": %.2f, \"high\": %.2f, \"low\": %.2f, \"close\": %.2f, \"priceLevels\": %s}", unixTimestampStr.GetChars(), sc.Open[recentBarIndex], sc.High[recentBarIndex], sc.Low[recentBarIndex], sc.Close[recentBarIndex], jsonArrayStr.c_str());
        SCString ResponseData;
        SCString RequestData;
        n_ACSIL::s_HTTPHeader Header;
        Header.Name = "Footprint";
        Header.Value = "application/json";
        sc.AddMessageToLog("-----------------", 0);
        if (!sc.MakeHTTPPOSTRequest("http://127.0.0.1:4000/api/footprint", RequestData.Format("Message=%s", jsonPayload.GetChars()), &Header, 1))
        {
            sc.AddMessageToLog("Error making HTTP request.", 0);
        }
    }
}