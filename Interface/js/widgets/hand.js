var HandWidget = function() {
    var ContainerId = 'hand_setting_container';
    var Heading = 'Hands';
var SettingsStr = [
				"<div class='widgetsettings' id='" + ContainerId + "'>",
					"<div class='setting'>",
						"Visualization",
						"<br>",
						"<span id='hands_visualization'>",
							"<input type='radio' id='hands_vis_hands' checked name='hands_vis_radio'/><label for='hands_vis_hands'>Arc</label>",
							"<input type='radio' id='hands_vis_points' name='hands_vis_radio'/><label for='hands_vis_points'>Points</label>",
						"</span>",
					"</div>",
					"<div class='setting'>",
						"OSC Message Name:",
						"<br>",
						"<input type='text' value='HandsMessage'/>",
					"</div>",
				"</div>"].join('\n');
    var activate = function () {
        $('#hands_visualization').buttonset();
    };
    var load = function (data) {
    };
    var save = function (data) {
    };
    return {
        type: "widget",
        settingsstr: SettingsStr, title: Heading, containerId: ContainerId, activate: activate,
            loadSettings: load, saveSettings: save
    };
};
