/**
 * @file wavefront.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief A file to provide the public interface for loading Wavefront
 * Object files, the model format Leto uses for testing.
 * @date 2024-09-25
 *
 * @copyright (c) 2024 - the Leto Team
 * This file is under the GNU Affero General Public License v3.0. That
 * means it is fully legal to distribute this source code and/or any
 * modified copies of this source code in both private and commercial
 * contexts. However, you must note copyright and state any changes you've
 * made. Source disclosure is also necessary. For more information, see the
 * @file LICENSE.md file that should have been distributed alongside this
 * source code.
 */

#ifndef LETO__WAVEFRONT__FORMAT
#define LETO__WAVEFRONT__FORMAT

// The model structural definitions, to provide standard structs for
// storing model info.
#include "../structure.h"

void LetoWavefrontProcessor(leto_model_t *model, const char *buffer);

void LetoMTLProcessor(const char *path);

void LetoUploadWavefront(leto_model_t *model, leto_mesh_t *mesh);

#endif // LETO__WAVEFRONT__FORMAT
