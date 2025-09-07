/*==============================================================================

	Aduio  [audio.h]
														 Author : Zishan
														 Date   : 2025/07/09
--------------------------------------------------------------------------------

==============================================================================*/
void InitAudio();
void UninitAudio();


int LoadAudio(const char* FileName);
void UnloadAudio(int Index);
void PlayAudio(int Index, bool Loop = false);

