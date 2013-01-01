// TODO: Error checking parser results
// TODO: Allow comments
// TODO: Allow base64 encoding for special characters
// Maybe use json instead of custom parser?

function parseKidget(s) {
    var ret = {};
    var t = ins.indexOf("(");
    var u = ins.lastIndexOf(")");

    var contents = $.trim(ins.substring(t+1,u)).split("|");
    for (var i = 0; i < contents.length; ++i) {
        var v = contents[i].indexOf(":");
        ret[$.trim(contents[i].substring(0,v))] = $.trim(contents[i].substring(v+1));
    }
    return ret;
}
function saveKidget(kidget) {
    var ret = "(";
    for (var x in kidget) {
        ret += x + ":" + kidget[x] + "|";
    }
    return ret.substr(0,ret.length-1) + ")";

}

var Kiblet = function(ins) {
    var kidgets = [];

    var that = {
        // Read/write functions
        load: function(s) {
            var t = ins.indexOf("{");
            var u = ins.lastIndexOf("}");
            var contents = $.trim(ins.substring(t+1, u)).split(",");
            for (var i = 0; i < contents.length; ++i) {
                if ($.trim(contents[i])) {
                    kidgets[i] = parseKidget($.trim(contents[i]));
                }
            }
        },
        save: function() {
            var ret = "{\n";
            for (var i = 0; i < kidgets.length; ++i) {
                ret += kidgets[i].save() + ",\n";
            }
            ret += "}";
        },
    };
    if (ins) that.load(ins);
    return that;
};

var Kib = function(s) {
    var kiblets = [];
    var name = s;
    var that = {
        save: function() {
            var ret = name + " [\n";
            for (var i = 0; i < kiblets.length; ++i) {
                ret += kiblets[i].save() + ";\n";
            }
            ret += "]";
            return ret;
        },
        load: function(ins) {
            var t = ins.indexOf("[");
            var u = ins.lastIndexOf("]");
            var contents = $.trim(ins.substring(t+1, u)).split(";");
            name = $.trim(ins.substring(0,t));
            for (var i = 0; i < contents.length-1; ++i) {
                kiblets[i] = Kiblet($.trim(contents[i]));
            }
        },
    };
    return that;
};
