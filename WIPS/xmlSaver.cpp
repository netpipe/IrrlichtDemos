 bool SaveToDisk(char* filename)
   {
      io::IWriteFile* file =  m_App->m_Engine->m_Device->getFileSystem()->createAndWriteFile(filename);
      if (file)
      {
         io::IXMLWriter* writer =  m_App->m_Engine->m_Device->getFileSystem()->createXMLWriter(file);
         if (!writer) { return false; }

         writer->writeXMLHeader();
         writer->writeLineBreak();

         writer->writeElement(L"CSLEVEL",false);
         writer->writeLineBreak();
         writer->writeLineBreak();
         writer->writeElement(L"CAMERA",false);
         writer->writeLineBreak();
         writer->writeElement(L"attributes", false);
         writer->writeLineBreak();
         writer->writeElement(L"Vector3df",true,L"name",L"Position",L"value",ConvertToWChar(Vector3dfToString(GetCamera()->getPosition())));
         writer->writeLineBreak();
         writer->writeElement(L"Vector3df",true,L"name",L"Target",L"value",ConvertToWChar(Vector3dfToString(GetCamera()->getTarget())));
         writer->writeLineBreak();
         writer->writeClosingTag(L"attributes");
         writer->writeLineBreak();
         writer->writeClosingTag(L"CAMERA");
         writer->writeLineBreak();

         writer->writeLineBreak();

         CSObject* o = GetFactory()->GetObjectManager()->GetNextObject(true);
         while(o)
         {
            io::IAttributes* attr = m_App->m_Engine->m_Device->getFileSystem()->createEmptyAttributes(LDRIVER);
            o->SerializeAttributes(attr);

            writer->writeElement(L"CSOBJECT",false,L"Object_Type",ConvertToWChar(o->GetInfo()->GetName()));
            writer->writeLineBreak();

            if (attr->getAttributeCount() != 0)
            {
               attr->write(writer);
            }
            attr->drop();
            writer->writeClosingTag(L"CSOBJECT");
            writer->writeLineBreak();
            writer->writeLineBreak();

            o = GetFactory()->GetObjectManager()->GetNextObject(false);
         }

         writer->writeClosingTag(L"CSLEVEL");
         writer->writeLineBreak();

         writer->drop();
      }
      file->drop();
      return true;
   } 
