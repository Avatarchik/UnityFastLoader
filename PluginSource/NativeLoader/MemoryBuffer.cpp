#include "MemoryBuffer.h"
using NativeLoader;

MemoryBuffer &MemoryBuffer::Create(int size){
    MemoryBuffer *buffer = new MemoryBuffer( sixe );
    return (*buffer);
}

void* MemoryBuffer::GetData(int index)const{
    char *ptr = reinterpret_cast<char*>(m_workingMemory);
    ptr = ptr + index;
    return ptr;
}
int MemoryBuffer::GetDataSize()const{
    return m_ptrIndex;
}


void MemoryBuffer::ResetData(){
    m_ptrIndex = 0;
}

void MemoryBuffer::AppendData( const void *ptr,int size){
    PrepareForDynamicAppend( size );
    memcpy( GetData(m_ptrIndex) ,ptr, size);
    m_ptrIndex = size;
}


void MemoryBuffer::PrepareForDynamicAppend(int size){
    if( m_ptrIndex + size > m_workingMemorySize ){
        ReSize( m_ptrIndex + size );
    }
}

void *MemoryBuffer::GetNextAppendPtr(){
    return GetData(m_ptrIndex);
}

int MemoryBuffer::NextPtr( int size ){
    m_ptrIndex += size;
}


void MemoryBuffer::Shrink(int size ){
    void *newWorking = malloc( size );
    memcpy( newWorking,m_workingMemory , size );
    free(m_workingMemory);

    m_workingMemory = newWorking;
    m_workingMemorySize = size;
}

void MemoryBuffer::ReSize(int size){
    void *newWorking = malloc( size );
    memcpy( newWorking,m_workingMemory , GetDataSize() );
    free(m_workingMemory);

    m_workingMemory = newWorking;
    m_workingMemorySize = size;
}

void MemoryBuffer::IncReferenceCount(){
    ++ m_referenceCount;
}
void MemoryBuffer::DecReferenceCount(){
    -- m_referenceCount;
    if( m_referenceCount <= 0 ){
        delete( this );
    }
}

MemoryBuffer::MemoryBuffer(int size):
m_workingMemory(NULL),
m_workingMemorySize(0),
m_referenceCount(0),
m_ptrIndex(0){
    m_workingMemory = malloc( size );
    m_workingMemorySize = size;
}


MemoryBuffer::~MemoryBuffer(){
    if( m_workingMemory ){
        free(m_workingMemory);
    }
}

