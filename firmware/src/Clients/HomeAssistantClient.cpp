#include "HomeAssistantClient.h"

HomeAssistantClient::HomeAssistantClient(Recuperation* recuperation, ILogger* logger, MqttClient* client, Settings* settings)
{
    this->recuperation = recuperation;
    this->logger = logger;
    this->client = client;
    this->settings = settings;
}

String HomeAssistantClient::getChipId() {
    uint32_t id = (uint32_t)ESP.getEfuseMac();
    char buf[9];
    sprintf(buf, "%06X", id & 0xFFFFFF);
    return String(buf);
}

void HomeAssistantClient::publishDiscoveryConfig()
{
    StaticJsonDocument<1024> d;
    String chipId = this->getChipId();

    this->logger->debug("Chip ID: " + chipId);

    // core entity data ---------------------------------------------------------
    d["name"]          = "Belimo CRA-B1P Ventilation";
    d["unique_id"]     = "belimo_crab1p_" + chipId;
    d["availability_topic"]     = settings->mqttAvaibilityTopic;
    d["payload_available"]      = "online";
    d["payload_not_available"]  = "offline";

    // command / state topics ---------------------------------------------------
    d["command_topic"]          = settings->mqttBaseTopic + "/fan/set";           // optional
    d["state_topic"]            = settings->mqttBaseTopic + "/fan/state";
    d["preset_mode_command_topic"] = settings->mqttBaseTopic + "/fan/set_preset";
    d["preset_mode_state_topic"]   = settings->mqttBaseTopic + "/fan/preset";
    d["percentage_command_topic"]  = settings->mqttBaseTopic + "/fan/set_percent";
    d["percentage_state_topic"]    = settings->mqttBaseTopic + "/fan/percent";

    // we expose three discrete presets
    JsonArray pm = d.createNestedArray("preset_modes");
    pm.add("low");
    pm.add("comfortable");
    pm.add("max");

    // Home-Assistant device registry info --------------------------------------
    JsonObject dev = d.createNestedObject("device");
    dev["identifiers"]  = chipId;
    dev["manufacturer"] = "Belimo";
    dev["model"]        = "CRA-B1P";
    dev["name"]         = "Belimo CRA-B1P";
    dev["sw_version"]   = FW_VERSION;

    // ship it ------------------------------------------------------------------
    String discoveryTopic = "homeassistant/fan/" + chipId + "/config";
    
    char payload[1024];
    size_t n = serializeJson(d, payload, sizeof(payload));

    const uint8_t* castedPayload = reinterpret_cast<const uint8_t*>(payload);
    bool ok = client->publish(discoveryTopic.c_str(), castedPayload, (unsigned int)n, true);

    bool onlineOk = this->client->publish(settings->mqttAvaibilityTopic.c_str(), "online", true);

    logger->debug("HA discovery publish: " + String(ok) + "online publish: " + String(onlineOk));
}

void HomeAssistantClient::receiveStates(char* topic, byte* payload, unsigned int len)
{
    payload[len] = '\0';
    String t(topic), msg((char*)payload);

    uint8_t speedState = 0;

    if (t.endsWith("/fan/set_preset"))
    {
        this->logger->debug("Setting " + msg + " by preset from Home Assistant.");

        if (msg == "low")               speedState = 1;
        else if (msg == "comfortable")  speedState = 2;
        else if (msg == "max")          speedState = 3;
    }
    else if (t.endsWith("/fan/set_percent"))
    {
        this->logger->debug("Setting " + msg + "% from Home Assistant.");

        int p = msg.toInt();

        if (p <= 0) speedState = 1;
        else if (p <= 50) speedState = 2;
        else speedState = 3;
    }

    this->recuperation->setState(speedState);
}

void HomeAssistantClient::publishStates(uint8_t speedState)
{
    const char* preset = "low";
    int pct = 0;

    if (speedState == 2)
    {
        this->logger->debug("Publishing state 2.");
        preset = "comfortable";
        pct = 50;
    }
    else if (speedState == 3)
    {
        this->logger->debug("Publishing state 3.");
        preset = "max";
        pct = 100;
    }

    if (!this->client->publish((settings->mqttBaseTopic + "/fan/state").c_str(), "ON",  true))
    {
        logger->error("Publish to /fan/state failed.");
    }

    if (!this->client->publish((settings->mqttBaseTopic + "/fan/preset").c_str(), preset, true))
    {
        logger->error("Publish to /fan/preset failed.");
    }

    if (!this->client->publish((settings->mqttBaseTopic + "/fan/percent").c_str(), String(pct).c_str(), true))
    {
        logger->error("Publish to /fan/percent failed.");
    }
}