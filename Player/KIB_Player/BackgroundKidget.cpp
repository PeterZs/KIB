#include "BackgroundKidget.h"

static double vscale = 0.0007;
static double vconst = 0.005;
static int new_star_rate = 180;

enum {
	FG_IMAGE,
	FG_SILHOUETTE,
	FG_SKELETON,
	FG_POLYGONS
};

enum {
	BG_IMAGE,
	BG_COLOR,
	BG_GRADIENT,
	BG_STARFIELD,
	BG_PSYCHEDELIC
};

BackgroundKidget::BackgroundKidget(void)
{
}

BackgroundKidget::BackgroundKidget(Json::Value* v)
{
	if ((*v)["avatar_style_picker"].asString() == "avatar_style_image") {
		fgvis = FG_IMAGE;
	} else if ((*v)["avatar_style_picker"].asString() == "avatar_style_silhouette") {
		fgvis = FG_SILHOUETTE;
	} else if ((*v)["avatar_style_picker"].asString() == "avatar_style_skeleton") {
		fgvis = FG_SKELETON;
	} else if ((*v)["avatar_style_picker"].asString() == "avatar_style_polygons") {
		fgvis = FG_POLYGONS;
	} else {
		throw new KIFormatException();
	}
	if ((*v)["bg_style_picker"].asString() == "bg_style_stars") {
		bgvis = BG_STARFIELD;
	} else if ((*v)["bg_style_picker"].asString() == "bg_style_image") {
		bgvis = BG_IMAGE;
	} else if ((*v)["bg_style_picker"].asString() == "bg_style_solid") {
		bgvis = BG_COLOR;
	} else if ((*v)["bg_style_picker"].asString() == "bg_style_gradient") {
		bgvis = BG_GRADIENT;
	} else if ((*v)["bg_style_picker"].asString() == "bg_style_psychedelic") {
		bgvis = BG_PSYCHEDELIC;
	} else {
		throw new KIFormatException();
	}

	parseColor((*v)["fg_style_color"].asString(), fg);
	parseColor((*v)["bg_style_color"].asString().substr(0,7), bg[0]);
	parseColor((*v)["bg_style_color"].asString().substr(8,7), bg[1]);

	width=640;
	height=480;
	data = new unsigned char[4*width*height];
	grad = new float[width*height];
	colorspaceplayer = new char[width*height];
	float maxdist = (float) sqrt((double)(width*width + height*height))/2.;
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			grad[i*width+j] = (float) sqrt((double)((i - height/2)*(i-height/2) + (j-width/2)*(j-width/2)))/maxdist;
		}
	}

	for (int i = 0; i < 2*new_star_rate; ++i) {
		starsr.push_back(randD()*maxdist);
		starst.push_back(randD()*2*M_PI);
		starsv.push_back((randD()*3+1)*vscale);
	}
	last_update = timeGetTime()-10;
	glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (GLvoid*) data);
    glBindTexture(GL_TEXTURE_2D, 0);

	// OpenGL setup
    glClearColor(0,0,0,0);
    glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0f);

}

BackgroundKidget::~BackgroundKidget(void)
{
}


void BackgroundKidget::draw(const Vector4* skeletonPositions, unsigned char* rgb, float* depth, char* pid){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho(0, width, height, 0, 1, -1);
    //gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	memcpy(data, rgb, 4*width*height);
	memset(colorspaceplayer, 0, width*height);
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			data[4*(i*width+j)+3] = 255;
			if (pid[i*width+j] != 0) {
				long x, y;
				NuiImageGetColorPixelCoordinatesFromDepthPixelAtResolution(NUI_IMAGE_RESOLUTION_640x480, NUI_IMAGE_RESOLUTION_640x480, NULL,
																		   j, i, depth[i*width+j], &x, &y);  
				colorspaceplayer[y*width+x] = 1;
			}
		}
	}
	// Generate gradient
	if (bgvis == BG_GRADIENT) {
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				if (!colorspaceplayer[i*width+j] || fgvis == FG_SKELETON) {
					for (int k = 0; k < 3; ++k) {
						data[4*(i*width+j)+k] = (char) (grad[i*width+j]*bg[0][k] + (1-grad[i*width+j])*bg[1][k]);    // Set foreground silhouette to foreground color
					}
				}
			}
		}
	}

	// Mask out background
	if (bgvis != BG_IMAGE && bgvis != BG_GRADIENT) {
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				if (!colorspaceplayer[i*width+j] || fgvis == FG_SKELETON) {
					data[4*(i*width+j)+3] = 0;    // Mask out background
				} else {
					data[4*(i*width+j)+3] = 255;
				}
			}
		}
	}

	// Color in foreground pixels
	if (fgvis == FG_SILHOUETTE) {
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				if (colorspaceplayer[i*width+j]) {
					for (int k = 0; k < 3; ++k) {
						data[4*(i*width+j)+k] = fg[2-k];    // Set foreground silhouette to foreground color
						data[4*(i*width+j)+3] = 255;
					}
				}
			}
		}
	}

	if (bgvis == BG_COLOR) {
		glBegin(GL_QUADS);
			glColor3f(bg[0][0]/255.f, bg[0][1]/255.f, bg[0][2]/255.f);
			glVertex3f(0.0f,0.0f, 1.0f);
			glVertex3f(width, 0.0f, 1.0f);
			glVertex3f(width, height, 1.0f);
			glVertex3f(0.0f, height, 1.0f);
		glEnd();
	} else if (bgvis == BG_STARFIELD) {
		glBegin(GL_QUADS);
			glColor3f(0.0f,0.0f,0.0f);
			glVertex3f(0.0f,0.0f, 1.0f);
			glVertex3f(width, 0.0f, 1.0f);
			glVertex3f(width, height, 1.0f);
			glVertex3f(0.0f, height, 1.0f);
		glEnd();

		unsigned long currtime = timeGetTime();
		unsigned long dt = currtime - last_update;
		last_update = currtime;
		glColor3f(1.f,1.f,1.f);
		glPointSize(2.f);
		glBegin(GL_POINTS);
		// Update star positions
		int ns = starsr.size();
		for (int i = 0; i < ns;) {
			starsr[i] += dt*(starsv[i]*starsr[i] + vconst);
			double x = width/2 + starsr[i]*sin(starst[i]);
			double y = height/2 + starsr[i]*cos(starst[i]);
			if (x < 0 || x > width || y < 0 || y > height) {
				starsr[i] = starsr.back(); starsr.pop_back();
				starst[i] = starst.back(); starst.pop_back();
				starsv[i] = starsv.back(); starsv.pop_back();
				--ns;
			} else {
				glVertex3f(x, y, 0.9f);
				++i;
			}
		}
		for (int i = 0; i < new_star_rate*dt/1000; ++i) {
			starsr.push_back(randD()*10);
			starst.push_back(randD()*2*M_PI);
			starsv.push_back((randD()*3+1)*vscale);
		}
		glEnd();
	} else if (bgvis == BG_PSYCHEDELIC) {
		// TODO: Implement psychedelic background
	} 
	
	// Copy data to texture and render it
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, textureId);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (GLvoid*)data);
	glColor3f(1.f,1.f,1.f);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(width, 0.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(width, height, 0.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(0.0f, height, 0.0f);

		/*glTexCoord2f(0.0f, 0.0f);
        glVertex2f(0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(width, 0.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(width, height);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(0.0f, height);*/
    glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	if (fgvis == FG_SKELETON) {
		USHORT tmpz;
		long tmpx, tmpy;
		for (int i = 0; i < NUI_SKELETON_POSITION_COUNT; ++i) {
			NuiTransformSkeletonToDepthImage(skeletonPositions[i], &tmpx, &tmpy, &tmpz, NUI_IMAGE_RESOLUTION_640x480);
			NuiImageGetColorPixelCoordinatesFromDepthPixelAtResolution(NUI_IMAGE_RESOLUTION_640x480, NUI_IMAGE_RESOLUTION_640x480, NULL,
																	   tmpx, tmpy, tmpz, &px[i], &py[i]); 
		}
		glColor3f(fg[0]/255.f, fg[1]/255.f, fg[2]/255.f);
		glLineWidth(2.f);
		glBegin(GL_LINES);
		drawLine(px, py, NUI_SKELETON_POSITION_HEAD, NUI_SKELETON_POSITION_SHOULDER_CENTER);
		drawLine(px, py, NUI_SKELETON_POSITION_HIP_CENTER, NUI_SKELETON_POSITION_SHOULDER_CENTER);
		// Left arm
		drawLine(px, py, NUI_SKELETON_POSITION_SHOULDER_LEFT, NUI_SKELETON_POSITION_SHOULDER_CENTER);
		drawLine(px, py, NUI_SKELETON_POSITION_SHOULDER_LEFT, NUI_SKELETON_POSITION_ELBOW_LEFT);
		drawLine(px, py, NUI_SKELETON_POSITION_WRIST_LEFT, NUI_SKELETON_POSITION_ELBOW_LEFT);
		drawLine(px, py, NUI_SKELETON_POSITION_WRIST_LEFT, NUI_SKELETON_POSITION_HAND_LEFT);
		// Right arm
		drawLine(px, py, NUI_SKELETON_POSITION_SHOULDER_RIGHT, NUI_SKELETON_POSITION_SHOULDER_CENTER);
		drawLine(px, py, NUI_SKELETON_POSITION_SHOULDER_RIGHT, NUI_SKELETON_POSITION_ELBOW_RIGHT);
		drawLine(px, py, NUI_SKELETON_POSITION_WRIST_RIGHT, NUI_SKELETON_POSITION_ELBOW_RIGHT);
		drawLine(px, py, NUI_SKELETON_POSITION_WRIST_RIGHT, NUI_SKELETON_POSITION_HAND_RIGHT);
		// Left leg
		drawLine(px, py, NUI_SKELETON_POSITION_HIP_LEFT, NUI_SKELETON_POSITION_HIP_CENTER);
		drawLine(px, py, NUI_SKELETON_POSITION_HIP_LEFT, NUI_SKELETON_POSITION_KNEE_LEFT);
		drawLine(px, py, NUI_SKELETON_POSITION_ANKLE_LEFT, NUI_SKELETON_POSITION_KNEE_LEFT);
		drawLine(px, py, NUI_SKELETON_POSITION_ANKLE_LEFT, NUI_SKELETON_POSITION_FOOT_LEFT);
		// Right leg
		drawLine(px, py, NUI_SKELETON_POSITION_HIP_RIGHT, NUI_SKELETON_POSITION_HIP_CENTER);
		drawLine(px, py, NUI_SKELETON_POSITION_HIP_RIGHT, NUI_SKELETON_POSITION_KNEE_RIGHT);
		drawLine(px, py, NUI_SKELETON_POSITION_ANKLE_RIGHT, NUI_SKELETON_POSITION_KNEE_RIGHT);
		drawLine(px, py, NUI_SKELETON_POSITION_ANKLE_RIGHT, NUI_SKELETON_POSITION_FOOT_RIGHT);
		glEnd();
	}
}