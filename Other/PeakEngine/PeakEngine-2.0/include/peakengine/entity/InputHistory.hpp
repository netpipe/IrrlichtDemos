/*
Copyright (c) 2009, Mathias Gottschlag

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef _PEAKENGINE_ENTITY_INPUTHISTORY_HPP_
#define _PEAKENGINE_ENTITY_INPUTHISTORY_HPP_

namespace peak
{
	/**
	 * Input frame to be used with InputHistory.
	 */
	template<typename T> struct InputHistoryFrame
	{
		/**
		 * Time of the input data.
		 */
		unsigned int time;
		/**
		 * Input data.
		 */
		T data;
		/**
		 * Next (newer) input frame or 0 if this is the end of the linked list.
		 * Does not have to be initialized prior to InputHistory::insert().
		 */
		InputHistoryFrame *next;
	};
	/**
	 * Linked list for storing the last input frames for client prediction.
	 */
	template<typename T> class InputHistory
	{
		public:
			/**
			 * Constructor.
			 */
			InputHistory() : frames(0)
			{
			}
			/**
			 * Destructor.
			 */
			~InputHistory()
			{
				// Delete remaining input frames
				while (frames)
				{
					InputHistoryFrame<T> *frame = frames;
					frames = frame->next;
					delete frame;
				}
			}

			/**
			 * Inserts an input frame into the input history list.
			 */
			void insert(InputHistoryFrame<T> *frame)
			{
				if (!frames)
				{
					// We only have one input frame
					frames = frame;
					last = frame;
					frame->next = 0;
				}
				// Insert input frame at the end of the list
				last->next = frame;
				frame->next = 0;
				last = frame;
			}

			/**
			 * Removes all input frames which are not relevant any more after
			 * the server has already received a certain client update. Also
			 * changes the time of the oldest still relevant inpit frame to
			 * match the given time.
			 * @param time Time of the last definately received client update.
			 */
			void removeIrrelevant(unsigned int time)
			{
				if (!frames)
					return;
				// Remove input frames which are older than time
				while (frames->next && frames->time < time
					&& frames->next->time <= time)
				{
					InputHistoryFrame<T> *frame = frames;
					frames = frame->next;
					delete frame;
				}
				// Set the time of the oldest input frame
				frames->time = time;
			}
			/**
			 * Returns the first relevant input frame.
			 */
			InputHistoryFrame<T> *get()
			{
				return frames;
			}
		private:
			InputHistoryFrame<T> *frames;
			InputHistoryFrame<T> *last;
	};
}

#endif
