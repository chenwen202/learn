#ifndef _SELF_DEFINE_SORT_HEADER_
namespace Util
{
    #define IN
    #define OUT
    #define IO

    class GlobalFunc{
        public:
            #pragma region _c_b_t_
            static int iParent(IN int index);
            static int iLeftChild(IN int index);
            static int iRightChild(IN int index);
            #pragma endregion _c_b_t_

            #pragma region _common_
            static int random_integers(IN int max, IN int min,IO int data[], IN int count);
            #pragma endregion _common_
    };

    class SortBase
    {
    public:
        SortBase() {}
        ~SortBase() {}

        //@brief make the input array sorted 
        //param[in,out] array, values to be sorted
        //param[in] count, len of array
        //@return error code
        virtual int SortFunc(IN OUT int array[], IN int count) = 0;
    };

    class HeapSort: public SortBase 
    {
       public:
            enum HeapType
            {
                SHIFT_DOWN,
                SHIFT_UP
            };

            HeapSort(HeapType eType = SHIFT_UP);
            ~HeapSort(){} 

            
            //@brief sorting using heapsort algorithm
            //param[in,out] array, values to be sorted
            //param[in] count, len of array
            //@return error code
            virtual int SortFunc(IN OUT int array[], IN int count);
        private:
            //@brief build a max-heap binary tree
            //param[in,out] array, values to be sorted
            //param[in] count, len of array
            //@return error code
            int Heapify(IN int array[], IN int count);
            //@brief move the biggest to the root node
            //param[in,out] array, values to be sorted
            //param[in] start, the node index of the array
            //param[in] end, the last index of the array
            //@return error code
            int ShiftDown(IN OUT int array[], IN int start, IN int end); 

            //@brief move the biggest to the root node
            //param[in,out] array, values to be sorted
            //param[in] start, the node index of the array
            //param[in] end, the last index of the array
            //@return error code
            int ShiftUp(IN OUT int array[], IN int start, IN int end); 
        private:
            HeapType    mHeapType;

    };

} // namespace Util

#endif // !_SELF_DEFINE_SORT_HEADER_