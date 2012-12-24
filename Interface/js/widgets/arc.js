var ArcWidget = function() {
    var ContainerId = 'arc_setting_container';
    var Heading = 'Arc';
var SettingsStr = [
				"<div class='widgetsettings' id='" + ContainerId + "'>",
					"<div class='setting'>",
						"Discreteness:",
						"<div id='arc_discrete_slider' width=200px>",
						"</div><span id='arc_discrete_label'>Continuous</span>",
					"</div>",
					"<div class='setting'>",
						"Visualization",
						"<br>",
						"<div id='arc_visualization'>",
							"<input type='radio' checked id='arc_vis_button' name='arc_vis_radio'/><label for='arc_vis_button'>Buttons</label>",
							"<input type='radio' id='arc_vis_arc' name='arc_vis_radio'/><label for='arc_vis_arc'>Arc</label>",
							"<input type='radio' id='arc_vis_points' name='arc_vis_radio'/><label for='arc_vis_points'>Points</label>",
						"</div>",
					"</div>",
					"<div class='setting'>",
						"OSC Message Name:",
						"<br>",
						"<input type='text' value='ArcMessage'/>",
					"</div>",
				"</div>"].join('\n');
    var activate = function () {
        $('#arc_visualization').buttonset();
        $('#arc_discrete_slider').slider({
            value:0,
            min:0,
            max:6,
            step:1,
            slide: function(ev, ui) {
                $('#arc_discrete_label').html(ui.value==0?"Continuous":ui.value);
            }
        });
    };
    var load = function (data) {
    };
    var save = function (data) {
    };
    return {
        type: "widget",
        settingsstr: SettingsStr, containerId: ContainerId, title: Heading, activate: activate,
            loadSettings: load, saveSettings: save
    };
};
