// From: http://jeshua.me/blog/SamplingUnivariateGaussianinJavascript
Math.normrnd = function(mean,std) { 
    if(this.extra == undefined){         
        var u,v;var s = 0; 
        while(s >= 1 || s == 0){ 
            u = Math.random()*2 - 1; v = Math.random()*2 - 1; 
            s = u*u + v*v; 
        } 
        var n = Math.sqrt(-2 * Math.log(s)/s); 
        this.extra = v * n; 
        return mean + u * n * std; 
    } else{ 
        var r = mean + this.extra * std; 
        this.extra = undefined; 
        return r; 
    } 
}
function scale(color, brightness) {
    var ret = [];
    for (var i = 0; i < 3; ++i) {
        ret[i] = Math.floor(color[i]*brightness);
        if (ret[i] > 255) ret[i] = 255;
        if (ret[i] < 0) ret[i] = 0;
    }
    return ret;
}
function toRgb(color) {
    if (!color) return [0,0,0];
    var r = parseInt(color.substr(1,2), 16);
    var g = parseInt(color.substr(3,2), 16);
    var b = parseInt(color.substr(5,2), 16);
    return [r,g,b];
}
function rgba(rgb, a) {
    return "rgba(" + rgb[0] + "," + rgb[1] + "," + rgb[2] + "," + a + ")";
}

function renderGlowball(p, r, ctx, color) {
    ctx.globalCompositeOperation = 'lighter';
    var gradient = ctx.createRadialGradient(p[0], p[1], 0, p[0], p[1], r);
    gradient.addColorStop(0, rgba(color,0.9))
    gradient.addColorStop(0.5, rgba(color,0.9))
    gradient.addColorStop(1, rgba(color,0))
    ctx.fillStyle = gradient;
    ctx.beginPath();
    ctx.arc(p[0], p[1], r, 0, Math.PI*2);
    ctx.fill();
    ctx.globalCompositeOperation = 'source-over';
}

var leftradius = 30;
var rightradius = 30;
var leftbrightness = 1;
var rightbrightness = 1;
var center = 30;
var speed = 80;
var radius_last_updated = 0;

function renderHands(widget, skeleton, ctx, w, h) {
    var color = toRgb(widget['hands_color']);
    var currtime = (new Date()).getTime();
    var dt = currtime - radius_last_updated;
    if (dt > speed) {
        var dl = Math.normrnd(leftradius-center, 0.4);
        var dr = Math.normrnd(rightradius-center, 0.4);
        var dlb = Math.normrnd(leftbrightness-1,0.06);
        var drb = Math.normrnd(rightbrightness-1,0.06);
        leftradius -= dl;
        rightradius -= dr;
        rightbrightness -= drb;
        leftbrightness -= dlb;

        radius_last_updated = currtime;
    }
    var leftcolor = scale(color, leftbrightness);
    var rightcolor = scale(color, rightbrightness);
    if (widget['hands_visualization'] == 'hands_vis_neon') {
        renderGlowball(skeleton.joint('lefthand', w, h), leftradius, ctx, leftcolor);
        renderGlowball(skeleton.joint('righthand', w, h), rightradius, ctx, rightcolor);
    } else if (widget['hands_visualization'] == 'hands_vis_fire') {
    } else if (widget['hands_visualization'] == 'hands_vis_lightning') {
        renderGlowball(skeleton.joint('lefthand', w, h), leftradius, ctx, leftcolor);
        renderGlowball(skeleton.joint('righthand', w, h), rightradius, ctx, rightcolor);
    }
}
