import face_recognition
import numpy as np

# Create arrays of known face encodings and their names
known_face_encodings = []
known_face_names = []
for i in range(1,7):
    data_image = face_recognition.load_image_file("data/face_data/face_%d.png"%(i,))
    data_encoding = face_recognition.face_encodings(data_image)[0]
    known_face_encodings.append(data_encoding)
    known_face_names.append(i)

def recogize_portrait(portrait_rgb):
    """
    @return The id (int) of face index. -1 if no match faces.
    """
    face_locations = [(0, portrait_rgb.shape[1], portrait_rgb.shape[0], 0)]
    face_encodings = face_recognition.face_encodings(portrait_rgb, face_locations)

    # Loop through each face in this frame of video
    for (top, right, bottom, left), face_encoding in zip(face_locations, face_encodings):
        # See if the face is a match for the known face(s)
        matches = face_recognition.compare_faces(known_face_encodings, face_encoding,0.6)

        name = -1
        # If a match was found in known_face_encodings, just use the first one.
        # if True in matches:
        #     first_match_index = matches.index(True)
        #     name = known_face_names[first_match_index]

        # Or instead, use the known face with the smallest distance to the new face
        face_distances = face_recognition.face_distance(known_face_encodings, face_encoding)
        best_match_index = np.argmin(face_distances)
        if matches[best_match_index]:
            name = known_face_names[best_match_index]
        return name
