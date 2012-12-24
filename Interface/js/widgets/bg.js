var BgWidget = function() {
    var ContainerId = 'bg_setting_container';
    var Heading = 'Background';
var SettingsStr = [
				"<div class='widgetsettings' id='" +  ContainerId + "'>",
				"<div class='setting' id='avatar_style'>",
						"Avatar style:",
						"<span id='avatar_style_picker'>",
							"<input type='radio' checked id='avatar_style_img' name='avatar_style_radio'/><label for='avatar_style_img'>Actual Image</label>",
							"<input type='radio' id='avatar_style_silhouette' name='avatar_style_radio'/><label for='avatar_style_silhouette'>Silhouette</label>",
							"<input type='radio' id='avatar_style_skeleton' name='avatar_style_radio'/><label for='avatar_style_skeleton'>Skeleton</label>",
							"<input type='radio' id='avatar_style_polygon' name='avatar_style_radio'/><label for='avatar_style_polygon'>Polygons</label>",
						"</span>",
					"</div>",
					"<div class='setting' id='bg_style'>",
						"Background style:",
						"<span id='bg_style_picker'>",
							"<input type='radio' checked id='bg_style_solid' name='bg_style_radio'/><label for='bg_style_solid'>Solid Color</label>",
							"<input type='radio' id='bg_style_gradient' name='bg_style_radio'/><label for='bg_style_gradient'>Gradient</label>",
							"<input type='radio' id='bg_style_stars' name='bg_style_radio'/><label for='bg_style_stars'>Starfield</label>",
							"<input type='radio' id='bg_style_psych' name='bg_style_radio'/><label for='bg_style_psych'>Psychedelic Color</label>",
						"</span>",
					"</div>",
					"<div class='setting'>",
						"Background Colors:",
						"<br>",
						"<div id='bg_style_colorpicker'></div>",
					"</div>",
					"<div class='setting'>",
						"Foreground Color:",
						"<br>",
						"<div id='fg_colorpicker'></div>",
					"</div>",
				"</div>"].join('\n');
    var activate = function () {
        $('#avatar_style_picker').buttonset();
        $('#bg_style_colorpicker').farbtastic();
        $('#fg_colorpicker').farbtastic();
        $('#bg_style_picker').buttonset();
    };
    var load = function (data) {
    };
    var save = function (data) {
    };
    return {
        type: "default",
        settingsstr: SettingsStr, containerId: ContainerId, title: Heading, activate: activate,
            loadSettings: load, saveSettings: save
    };
};
