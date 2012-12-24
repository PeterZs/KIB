var KibletWidget = function() {
    var ContainerId = 'kiblet_setting_container';
    var Heading = 'Kiblet';
var SettingsStr = [
"<div class='widgetsettings' id='"+ ContainerId + "'>",
					"<div class='setting' id='kiblet_trigger'>",
						"Trigger:",
						"<span id='kiblet_trigger'>",
							"<input type='radio' checked id='kiblet_trigger_time' name='kiblet_trigger_radio'/><label for='kiblet_trigger_time'>Time</label>",
							"<input type='radio' id='kiblet_trigger_event' name='kiblet_trigger_radio'/><label for='kiblet_trigger_event'>Event</label>",
						"</span>",
					"</div>",
					"<div class = 'setting'>",
					"<button class='kiblet_save'>Save Kiblet</button>",
					"</div>",
				"</div>"].join('\n');
    var activate = function () {
        $('#kiblet_trigger').buttonset();
        $('button.kiblet_save').button();
    };
    var load = function () {
    };
    var save = function (data) {
    };
    return {
        type: "default",
        settingsstr: SettingsStr, containerId: ContainerId, title: Heading, activate: activate,
            loadSettings: load, saveSettings: save
    };
};
